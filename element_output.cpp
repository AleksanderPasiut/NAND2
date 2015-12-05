#include "element_output.h"

ELEMENT_OUTPUT::ELEMENT_OUTPUT(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   IDWriteTextFormat* text_format,
							   float pos_x,
							   float pos_y,
							   unsigned id)
	: ELEMENT(target, brush_set, text_format, pos_x, pos_y, 80.0f, 50.0f, id, false)
{
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_OUTPUT::RetControlPoint() const
{
	return D2D1::Point2F(pos.x+0.7f*size.width,
						 pos.y+0.6f*size.height);
}
D2D1_POINT_2F ELEMENT_OUTPUT::RetInputPoint() const
{
	return D2D1::Point2F(pos.x+0.3f*size.width,
						 pos.y+0.6f*size.height);
}
void ELEMENT_OUTPUT::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetControlPoint(), CONTROL_RADIUS, CONTROL_RADIUS);
	return;
}
void ELEMENT_OUTPUT::RetInputEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetInputPoint(), PORT_RADIUS, PORT_RADIUS);
	return;
}

ELEMENT_OUTPUT* ELEMENT_OUTPUT::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   IDWriteTextFormat* text_format,
									   float pos_x,
									   float pos_y,
									   unsigned id)
{
	ELEMENT_OUTPUT* ret = new ELEMENT_OUTPUT(target,
											 brush_set,
											 text_format,
											 pos_x,
											 pos_y,
											 id);

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
		target->DrawLine(RetInputPoint(), end, brush->Red(), 2.0f, brush->Stroke());	}

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
	// usuni�cie starego outputa
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
	// usuwa siebie z listy (je�eli siebie nie by�o, to przerywa funkcje)
	if (!compute_list.remove_first_element())
		return;
	
	// ustawia sw�j stan
	state = input.target ? input.target->RetState(input.id) : EL_STATE_FALSE;
	return;
}

