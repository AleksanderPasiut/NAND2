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
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_NAND::RetControlPoint() const
{
	return D2D1::Point2F(pos.x+0.5f*size.width,
						 pos.y+(ia == 2 ? 0.6f : 0.5f)*size.height);
}
D2D1_POINT_2F ELEMENT_NAND::RetInputPoint(unsigned i) const
{
	return D2D1::Point2F(pos.x+0.16f*size.width,
						 pos.y+size.height*(static_cast<float>(i+1)/static_cast<float>(ia+1)));
}
D2D1_POINT_2F ELEMENT_NAND::RetOutputPoint() const
{
	return D2D1::Point2F(pos.x+0.84f*size.width,
						 pos.y+size.height*(ia == 2 ? 0.6f : 0.5f));
}
void ELEMENT_NAND::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetControlPoint(), CONTROL_RADIUS, CONTROL_RADIUS);
	return;
}
void ELEMENT_NAND::RetInputEllipse(D2D1_ELLIPSE& out, unsigned i) const
{
	out = D2D1::Ellipse(RetInputPoint(i), PORT_RADIUS, PORT_RADIUS);
	return;
}
void ELEMENT_NAND::RetOutputEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetOutputPoint(), PORT_RADIUS, PORT_RADIUS);
	return;
}

ELEMENT_NAND* ELEMENT_NAND::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   float pos_x,
									   float pos_y,
									   unsigned id,
									   unsigned input_amount)
{
	ELEMENT_NAND* ret = new ELEMENT_NAND(target,
										 brush_set,
										 pos_x,
										 pos_y,
										 id,
										 input_amount);

	if (ret)
	{
		return ret;
	}
	
	return 0;
}

EVPV ELEMENT_NAND::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	for (unsigned i = 0; i < ia; i++)
	{	RetInputEllipse(ellipse, i);
		if (PointInEllipse(ellipse, click))
			return EVPV(EVPV_INPUT, i);		}

	RetOutputEllipse(ellipse);
	if (PointInEllipse(ellipse, click))
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

	RetOutputEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());

	for (unsigned i = 0; i < ia; i++)
	{
		RetInputEllipse(ellipse, i);
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
			input[i].target->RetOutputPoint(end, input[i].id);
			target->DrawLine(RetInputPoint(i), end, brush->Red(), 2.0f, brush->Stroke());	}
	return;
}

bool ELEMENT_NAND::RetInputPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id >= ia)
		return false;

	out = RetInputPoint(id);
	return true;
}
bool ELEMENT_NAND::RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id)
		return false;

	out = RetOutputPoint();
	return true;
}

void ELEMENT_NAND::SetInput(unsigned this_input_id, ELEMENT* target, unsigned id)
{
	// je¿eli zadany input jest spoza zakresu
	if (this_input_id >= ia)
		return;

	// reset starego outputa
	if (input[this_input_id].target)
		input[this_input_id].target->DelOutput(id, this, this_input_id);

	// ustawienie nowego inputa
	input[this_input_id].target = target;
	input[this_input_id].id = id;

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
			input[i].id = 0;	}

	output_list.remove(target);
	return;
}

void ELEMENT_NAND::Proceed(OUTPUT_LIST& compute_list)
{
	// usuwa siebie z listy (je¿eli siebie nie by³o, to przerywa funkcje)
	if (!compute_list.remove_first_element())
		return;

	// ustawia swój stan
	state = EL_STATE_FALSE;
	for (unsigned i = 0; i < ia; i++)
		if (input[i].target && input[i].target->RetState(input[i].id) == EL_STATE_FALSE)
			state = EL_STATE_TRUE;

	// dodaje do listy wszystkie swoje outputy
	for (unsigned i = 0; i < output_list.retAmount(); i++)
		compute_list.add(output_list[i]->element, output_list[i]->input);

	return;
}
void ELEMENT_NAND::Reset()
{
	// reset niepod³¹czonych bramek
	for (unsigned i = 0; i < ia; i++)
		if (input[i].target)
			return;

	state = EL_STATE_FALSE;
	return;
}