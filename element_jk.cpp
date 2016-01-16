#include "element_jk.h"

ELEMENT_JK::ELEMENT_JK(ID2D1HwndRenderTarget* target,
	BRUSH_SET* brush_set,
	float pos_x,
	float pos_y,
	unsigned id)
	: ELEMENT(target, brush_set, pos_x, pos_y, 120.0f, 108.0f, id, true)
{
	propagation = false;
	state = EL_STATE_FALSE;
}

D2D1_POINT_2F ELEMENT_JK::RetControlPoint(unsigned i) const
{
	return D2D1::Point2F(pos.x + 0.5f*size.width,
		pos.y + size.height*(1.0f + i) / 3.0f);
}
D2D1_POINT_2F ELEMENT_JK::RetInputPortPoint(unsigned i) const
{
	return D2D1::Point2F(pos.x + 0.13f*size.width,
		pos.y + size.height*(i + 1.0f) / 6.0f);
}
D2D1_POINT_2F ELEMENT_JK::RetOutputPortPoint(unsigned i) const
{
	return D2D1::Point2F(pos.x + 0.87f*size.width,
		pos.y + size.height*(1.0f+i)/3.0f);
}
void ELEMENT_JK::RetControlEllipse(D2D1_ELLIPSE& out, unsigned i) const
{
	out = D2D1::Ellipse(RetControlPoint(i), CONTROL_RADIUS, CONTROL_RADIUS);
	return;
}
void ELEMENT_JK::RetInputPortEllipse(D2D1_ELLIPSE& out, unsigned i) const
{
	out = D2D1::Ellipse(RetInputPortPoint(i), PORT_RADIUS, PORT_RADIUS);
	return;
}
void ELEMENT_JK::RetOutputPortEllipse(D2D1_ELLIPSE& out, unsigned i) const
{
	out = D2D1::Ellipse(RetOutputPortPoint(i), PORT_RADIUS, PORT_RADIUS);
	return;
}
void ELEMENT_JK::DrawTexts() const
{
	wchar_t letters[] = L"SJCKRQ~Q";

	for (unsigned i = 0; i < 5; i++)
		target->DrawTextA(
			letters+i,
			1, 
			brush->SmallFont(), 
			D2D1::RectF(
				pos.x+0.21f*size.width, 
				pos.y+size.height*(i+1.0f) / 6.0f-7.0f,
				pos.x+0.21f*size.width+10.0f,
				pos.y+size.height*(i+1.0f) / 6.0f+5.0f),
			brush->Black());

	for (unsigned i = 0; i < 2; i++)
		target->DrawTextA(
			letters+5+i,
			i+1,
			brush->SmallFont(),
			D2D1::RectF(
				pos.x+0.8f*size.width-20.0f,
				pos.y+size.height*(i+1.0f)/3.0f-8.0f,
				pos.x+0.8f*size.width,
				pos.y+size.height*(i+1.0f)/3.0f+5.0f),
			brush->Black());

	return;
}

EVPV ELEMENT_JK::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	for (unsigned i = 0; i < 5; i++)
	{
		RetInputPortEllipse(ellipse, i);
		if (IsPointInEllipse(ellipse, click))
			return EVPV(EVPV_INPUT, i);
	}

	for (unsigned i = 0; i < 2; i++)
	{
		RetOutputPortEllipse(ellipse, i);
		if (IsPointInEllipse(ellipse, click))
			return EVPV(EVPV_OUTPUT, i);
	}

	return ELEMENT::MouseInput(click);
}
void ELEMENT_JK::Paint() const
{
	ELEMENT::Paint();

	D2D1_ELLIPSE ellipse;

	RetControlEllipse(ellipse, 0);
	target->FillEllipse(ellipse, state == EL_STATE_TRUE ? brush->Red() : brush->DarkRed());
	target->DrawEllipse(ellipse, brush->Gray());

	RetControlEllipse(ellipse, 1);
	target->FillEllipse(ellipse, state == EL_STATE_TRUE ? brush->DarkRed() : brush->Red());
	target->DrawEllipse(ellipse, brush->Gray());

	for (unsigned i = 0; i < 2; i++)
	{
		RetOutputPortEllipse(ellipse, i);
		target->FillEllipse(ellipse, brush->Black());
		target->DrawEllipse(ellipse, brush->Gray());
	}

	for (unsigned i = 0; i < 5; i++)
	{
		RetInputPortEllipse(ellipse, i);
		target->FillEllipse(ellipse, brush->Black());
		target->DrawEllipse(ellipse, brush->Gray());
	}

	DrawTexts();
	return;
}
void ELEMENT_JK::PaintWires() const
{
	for (unsigned i = 0; i < 5; i++)
		if (input[i].target)
		{
			D2D1_POINT_2F end;
			input[i].target->RetOutputPortPoint(end, input[i].output);
			target->DrawLine(RetInputPortPoint(i), end, brush->Red(), 2.0f, brush->Stroke());
		}
	return;
}

bool ELEMENT_JK::RetInputPortPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id >= 5)
		return false;

	out = RetInputPortPoint(id);
	return true;
}
bool ELEMENT_JK::RetOutputPortPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id >= 2)
		return false;

	out = RetOutputPortPoint(id);
	return true;
}

void ELEMENT_JK::SetInput(unsigned this_input_id, ELEMENT* target, unsigned id)
{
	// je¿eli zadany input jest spoza zakresu
	if (this_input_id >= 5)
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
void ELEMENT_JK::RemoveLinkage(ELEMENT* target)
{
	for (unsigned i = 0; i < 5; i++)
		if (input[i].target == target)
		{
			input[i].target = 0;
			input[i].output = 0;
		}

	output_list[0].remove(target);
	output_list[1].remove(target);
	return;
}

EL_STATE ELEMENT_JK::RetState(unsigned id) const
{
	if (id == 0)
		return state;
	else
	{	
		if (state == EL_STATE_TRUE)
			return EL_STATE_FALSE;
		else return EL_STATE_TRUE;
	}
}

bool ELEMENT_JK::Proceed(OUTPUT_LIST* next_list)
{
	if (!propagation)
	{
		propagation = true;
		for (unsigned i = 0; i < 5; i++)
			if (input[i].target)
				input[i].state = input[i].target->RetState(input[i].output);

		next_list->add(this, 0);

		return true;
	}

	EL_STATE old_state = state;
	
	propagation = false;

	if (input[0].target && input[0].state == EL_STATE_TRUE)
		state = EL_STATE_TRUE;
	else if (input[4].target && input[4].state == EL_STATE_TRUE)
		state = EL_STATE_FALSE;
	else if (input[2].target && input[2].state == EL_STATE_TRUE)
	{		
		if (input[1].state && input[1].state == EL_STATE_TRUE)
		{
			if (input[3].target && input[3].state == EL_STATE_TRUE)
			{
				if (state == EL_STATE_TRUE)
					state = EL_STATE_FALSE;
				else state = EL_STATE_TRUE;
			}
			else state = EL_STATE_TRUE;
		}
		else state = EL_STATE_FALSE;
	}

	for (unsigned j = 0; j < 2; j++)
		for (unsigned i = 0; i < output_list[j].retAmount(); i++)
			next_list->add_if_new(output_list[j][i]->element, output_list[j][i]->input);

	return old_state != state;
}