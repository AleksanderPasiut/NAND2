#include "element_output.h"

ELEMENT_OUTPUT::ELEMENT_OUTPUT(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   float pos_x,
							   float pos_y,
							   float width,
							   float height)
	: ELEMENT(target, brush_set, pos_x, pos_y, width, height, false)
{
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_OUTPUT::RetControlPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.7f*size.width),
						 ts.height*(pos.y+0.6f*size.height));
}
D2D1_POINT_2F ELEMENT_OUTPUT::RetInputPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.3f*size.width),
						 ts.height*(pos.y+0.6f*size.height));
}
void ELEMENT_OUTPUT::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.02f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetControlPoint(), radius, radius);
	return;
}
void ELEMENT_OUTPUT::RetInputEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.012f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetInputPoint(), radius, radius);
	return;
}

ELEMENT_OUTPUT* ELEMENT_OUTPUT::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   float pos_x,
									   float pos_y,
									   float width,
									   float height)
{
	ELEMENT_OUTPUT* ret = new ELEMENT_OUTPUT(target,
											 brush_set,
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

EVPV ELEMENT_OUTPUT::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	RetInputEllipse(ellipse);
	if (PointInEllipse(ellipse, click))
		return EVPV(EVPV_INPUT);

	return ELEMENT::MouseInput(click);
}
void ELEMENT_OUTPUT::Paint() const
{
	ELEMENT::Paint();

	D2D1_ELLIPSE ellipse;

	RetControlEllipse(ellipse);
	target->FillEllipse(ellipse, state == EL_STATE_TRUE ? brush->Red() : brush->DarkRed());
	target->DrawEllipse(ellipse, brush->Gray());

	RetInputEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());
	return;
}
void ELEMENT_OUTPUT::PaintWires() const
{
	if (input.target)
	{	D2D1_POINT_2F end;
		input.target->RetOutputPoint(end, input.id);
		target->DrawLine(RetInputPoint(), end, brush->Red(), 2.0f, 0);	}

	return;
}

bool ELEMENT_OUTPUT::RetInputPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id)
		return false;

	out = RetInputPoint();
	return true;
}

void ELEMENT_OUTPUT::SetInput(unsigned id, ELEMENT* target, unsigned target_id)
{
	// usuniêcie starego outputa
	if (input.target)
		input.target->DelOutput(input.id, this, 0);

	// ustawienie nowego inputa
	input.target = target;
	input.id = target_id;

	// ustawienie nowego outputa
	if (input.target)
		input.target->AddOutput(input.id, this, 0);

	return;
}
void ELEMENT_OUTPUT::RemoveLinkage(ELEMENT* target)
{
	if (input.target == target)
	{	input.target = 0;
		input.id = 0;	}
	return;
}

void ELEMENT_OUTPUT::Proceed(OUTPUT_LIST& compute_list)
{
	// usuwa siebie z listy (je¿eli siebie nie by³o, to przerywa funkcje)
	if (!compute_list.remove_first_element())
		return;
	
	// ustawia swój stan
	state = input.target ? input.target->RetState(input.id) : EL_STATE_FALSE;
	return;
}

