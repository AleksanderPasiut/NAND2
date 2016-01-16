#include "element_source.h"

ELEMENT_SOURCE::ELEMENT_SOURCE(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   float pos_x,
							   float pos_y,
							   unsigned id)
	: ELEMENT(target, brush_set, pos_x, pos_y, 80.0f, 50.0f, id, true),
	  state(EL_STATE_FALSE)
{}

D2D1_POINT_2F ELEMENT_SOURCE::RetControlPoint() const
{
	return D2D1::Point2F(pos.x+0.3f*size.width,
						 pos.y+0.6f*size.height);
}
D2D1_POINT_2F ELEMENT_SOURCE::RetOutputPortPoint() const
{
	return D2D1::Point2F(pos.x+0.7f*size.width,
						 pos.y+0.6f*size.height);
}
void ELEMENT_SOURCE::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetControlPoint(), CONTROL_RADIUS, CONTROL_RADIUS);
	return;
}
void ELEMENT_SOURCE::RetOutputPortEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetOutputPortPoint(), PORT_RADIUS, PORT_RADIUS);
	return;
}

EVPV ELEMENT_SOURCE::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	RetControlEllipse(ellipse);
	if (IsPointInEllipse(ellipse, click))
	{	state = static_cast<EL_STATE>(!static_cast<int>(state));
		return EVPV(EVPV_CONTROL); }

	RetOutputPortEllipse(ellipse);
	if (IsPointInEllipse(ellipse, click))
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

	RetOutputPortEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());
	return;
}

bool ELEMENT_SOURCE::RetOutputPortPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id)
		return false;

	out = RetOutputPortPoint();
	return true;
}

bool ELEMENT_SOURCE::Proceed(OUTPUT_LIST* next_list, unsigned int)
{
	for (unsigned i = 0; i < output_list.retAmount(); i++)
		next_list->add_if_new(output_list[i]->element, output_list[i]->input);

	return true;
}
