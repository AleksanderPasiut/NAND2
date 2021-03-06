#include "element_nand.h"

ELEMENT_NAND::ELEMENT_NAND(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   float pos_x,
							   float pos_y,
							   unsigned id,
							   unsigned input_amount)
	: ELEMENT(target, brush_set, pos_x, pos_y, 100.0f, (input_amount+1)*18.0f, id, false)
{
	ia = input_amount;
	propagation = false;
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_NAND::RetControlPoint() const
{
	return D2D1::Point2F(pos.x+0.5f*size.width,
						 pos.y+(ia == 2 ? 0.6f : 0.5f)*size.height);
}
D2D1_POINT_2F ELEMENT_NAND::RetInputPortPoint(unsigned i) const
{
	return D2D1::Point2F(pos.x+0.16f*size.width,
						 pos.y+size.height*(static_cast<float>(i+1)/static_cast<float>(ia+1)));
}
D2D1_POINT_2F ELEMENT_NAND::RetOutputPortPoint() const
{
	return D2D1::Point2F(pos.x+0.84f*size.width,
						 pos.y+size.height*(ia == 2 ? 0.6f : 0.5f));
}
void ELEMENT_NAND::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetControlPoint(), CONTROL_RADIUS, CONTROL_RADIUS);
	return;
}
void ELEMENT_NAND::RetInputPortEllipse(D2D1_ELLIPSE& out, unsigned i) const
{
	out = D2D1::Ellipse(RetInputPortPoint(i), PORT_RADIUS, PORT_RADIUS);
	return;
}
void ELEMENT_NAND::RetOutputPortEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetOutputPortPoint(), PORT_RADIUS, PORT_RADIUS);
	return;
}

EVPV ELEMENT_NAND::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	for (unsigned i = 0; i < ia; i++)
	{	RetInputPortEllipse(ellipse, i);
		if (IsPointInEllipse(ellipse, click))
			return EVPV(EVPV_INPUT, i);		}

	RetOutputPortEllipse(ellipse);
	if (IsPointInEllipse(ellipse, click))
		return EVPV(EVPV_OUTPUT);

	return ELEMENT::MouseInput(click);
}
void ELEMENT_NAND::Paint() const
{
	ELEMENT::Paint();

	D2D1_ELLIPSE ellipse;

	RetControlEllipse(ellipse);
	target->FillEllipse(ellipse, state == EL_STATE_TRUE ? brush->Red() : brush->DarkRed());
	target->DrawEllipse(ellipse, brush->Gray());

	RetOutputPortEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());

	for (unsigned i = 0; i < ia; i++)
	{
		RetInputPortEllipse(ellipse, i);
		target->FillEllipse(ellipse, brush->Black());
		target->DrawEllipse(ellipse, brush->Gray());
	}

	return;
}
void ELEMENT_NAND::PaintWires() const
{
	for (unsigned i = 0; i < ia; i++)
		if (input[i].target)
		{	D2D1_POINT_2F end;
			input[i].target->RetOutputPortPoint(end, input[i].output);
			target->DrawLine(RetInputPortPoint(i), end, brush->Red(), 2.0f, brush->Stroke());	}
	return;
}

bool ELEMENT_NAND::RetInputPortPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id >= ia)
		return false;

	out = RetInputPortPoint(id);
	return true;
}
bool ELEMENT_NAND::RetOutputPortPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id)
		return false;

	out = RetOutputPortPoint();
	return true;
}

void ELEMENT_NAND::SetInput(unsigned this_input_id, ELEMENT* target, unsigned id)
{
	// je�eli zadany input jest spoza zakresu
	if (this_input_id >= ia)
		return;

	// reset starego outputa
	if (input[this_input_id].target)
		input[this_input_id].target->DelOutput(id, this, this_input_id);

	// ustawienie nowego inputa
	input[this_input_id].target = target;
	input[this_input_id].output = id;

	// ustawienie nowego outputa
	if (input[this_input_id].target)
		input[this_input_id].target->AddOutput(id, this, this_input_id);

	return;
}
void ELEMENT_NAND::RemoveLinkage(ELEMENT* target)
{
	for (unsigned i = 0; i < ia; i++)
		if (input[i].target == target)
		{	input[i].target = 0;
			input[i].output = 0;	}

	output_list.remove(target);
	return;
}

bool ELEMENT_NAND::Proceed(OUTPUT_LIST* next_list)
{
	if (!propagation)
	{
		propagation = true;
		for (unsigned i = 0; i < ia; i++)
			if (input[i].target)
				input[i].state = input[i].target->RetState(input[i].output);

		next_list->add(this, 0);

		return true;
	}
	
	EL_STATE old_state = state;

	propagation = false;
	state = EL_STATE_FALSE;
	for (unsigned i = 0; i < ia; i++)
		if (input[i].target && input[i].state == EL_STATE_FALSE)
			state = EL_STATE_TRUE;
	
	for (unsigned i = 0; i < output_list.retAmount(); i++)
		next_list->add_if_new(output_list[i]->element, output_list[i]->input);

	return old_state != state;
}