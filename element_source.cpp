#include "element_source.h"

ELEMENT_SOURCE::ELEMENT_SOURCE(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   IDWriteTextFormat* text_format,
							   float pos_x,
							   float pos_y,
							   float width,
							   float height,
							   unsigned id)
	: ELEMENT(target, brush_set, text_format, pos_x, pos_y, width, height, id, true)
{
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_SOURCE::RetControlPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.3f*size.width),
						 ts.height*(pos.y+0.6f*size.height));
}
D2D1_POINT_2F ELEMENT_SOURCE::RetOutputPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.7f*size.width),
						 ts.height*(pos.y+0.6f*size.height));
}
void ELEMENT_SOURCE::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.02f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetControlPoint(), radius, radius);
	return;
}
void ELEMENT_SOURCE::RetOutputEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.012f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetOutputPoint(), radius, radius);
	return;
}

ELEMENT_SOURCE* ELEMENT_SOURCE::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   IDWriteTextFormat* text_format,
									   float pos_x,
									   float pos_y,
									   float width,
									   float height,
									   unsigned id)
{
	ELEMENT_SOURCE* ret = new ELEMENT_SOURCE(target,
											 brush_set,
											 text_format,
											 pos_x,
											 pos_y,
											 width,
											 height,
											 id);

	if (ret)
	{
		return ret;
	}
	
	return 0;
}

EVPV ELEMENT_SOURCE::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	RetControlEllipse(ellipse);
	if (PointInEllipse(ellipse, click))
	{	state = static_cast<EL_STATE>(!static_cast<int>(state));
		return EVPV(EVPV_CONTROL); }

	RetOutputEllipse(ellipse);
	if (PointInEllipse(ellipse, click))
		return EVPV(EVPV_OUTPUT);

	return ELEMENT::MouseInput(click);
}
void ELEMENT_SOURCE::Paint() const
{
	ELEMENT::Paint();

	D2D1_ELLIPSE ellipse;

	RetControlEllipse(ellipse);
	target->FillEllipse(ellipse, state == EL_STATE_TRUE ? brush->Red() : brush->DarkRed());
	target->DrawEllipse(ellipse, brush->Gray());

	RetOutputEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());
	return;
}

bool ELEMENT_SOURCE::RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id)
		return false;

	out = RetOutputPoint();
	return true;
}

void ELEMENT_SOURCE::Proceed(OUTPUT_LIST& compute_list)
{
	// usuwa siebie z listy (je�eli siebie nie by�o, to przerywa funkcje)
	if (!compute_list.remove_first_element())
		return;

	// dodaje do listy wszystkie swoje outputy
	for (unsigned i = 0; i < output_list.retAmount(); i++)
		compute_list.add(output_list[i]->element, output_list[i]->input);

	return;
}
