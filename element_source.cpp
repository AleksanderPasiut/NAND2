#include "element_source.h"

ELEMENT_SOURCE::ELEMENT_SOURCE(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   IDWriteTextFormat* text_format,
							   float pos_x,
							   float pos_y,
							   float width,
							   float height)
	: ELEMENT(target, brush_set, text_format, pos_x, pos_y, width, height)
{
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_SOURCE::RetControlPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.3f*size.width),
						 ts.height*(pos.y+size.height/2));
}
D2D1_POINT_2F ELEMENT_SOURCE::RetOutputPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.7f*size.width),
						 ts.height*(pos.y+size.height/2));
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
									   float height)
{
	ELEMENT_SOURCE* ret = new ELEMENT_SOURCE(target,
											 brush_set,
											 text_format,
											 pos_x,
											 pos_y,
											 width,
											 height);

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
	if (PointInCircle(ellipse, click))
	{	state = static_cast<EL_STATE>(!static_cast<int>(state));
		return EVPV(EVPV_CONTROL); }

	RetOutputEllipse(ellipse);
	if (PointInCircle(ellipse, click))
		return EVPV(EVPV_OUTPUT);

	return ELEMENT::MouseInput(click);
}
void ELEMENT_SOURCE::Paint()
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