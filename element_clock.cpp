#include "element_clock.h"
#include "master.h"

ELEMENT_CLOCK::ELEMENT_CLOCK(ID2D1HwndRenderTarget* target,
							 BRUSH_SET* brush_set,
							 float pos_x,
							 float pos_y,
							 unsigned id,
							 unsigned in_elapse,
							 MASTER* in_Master)
	: ELEMENT(target, brush_set, pos_x, pos_y, 80.0f, 70.0f, id, true)
{
	state = EL_STATE_FALSE;
	elapse = in_elapse;
	Master = in_Master;

	SetTimer(target->GetHwnd(), reinterpret_cast<UINT_PTR>(this), elapse, ClockElementTimerProc);
}
ELEMENT_CLOCK::~ELEMENT_CLOCK()
{
	KillTimer(target->GetHwnd(), reinterpret_cast<UINT_PTR>(this));
}

D2D1_POINT_2F ELEMENT_CLOCK::RetControlPoint() const
{
	return D2D1::Point2F(pos.x+0.3f*size.width,
						 pos.y+0.4f*size.height);
}
D2D1_POINT_2F ELEMENT_CLOCK::RetOutputPoint() const
{
	return D2D1::Point2F(pos.x+0.7f*size.width,
						 pos.y+0.4f*size.height);
}
void ELEMENT_CLOCK::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetControlPoint(), CONTROL_RADIUS, CONTROL_RADIUS);
	return;
}
void ELEMENT_CLOCK::RetOutputEllipse(D2D1_ELLIPSE& out) const
{
	out = D2D1::Ellipse(RetOutputPoint(), PORT_RADIUS, PORT_RADIUS);
	return;
}

ELEMENT_CLOCK* ELEMENT_CLOCK::Create(ID2D1HwndRenderTarget* target,
									 BRUSH_SET* brush_set,
									 float pos_x,
									 float pos_y,
									 unsigned id,
									 unsigned in_elapse,
									 MASTER* Master)
{
	ELEMENT_CLOCK* ret = new ELEMENT_CLOCK(target,
										   brush_set,
										   pos_x,
										   pos_y,
										   id,
										   in_elapse,
										   Master);

	if (ret)
	{
		return ret;
	}
	
	return 0;
}

EVPV ELEMENT_CLOCK::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_ELLIPSE ellipse;
	RetOutputEllipse(ellipse);
	if (PointInEllipse(ellipse, click))
		return EVPV(EVPV_OUTPUT);

	return ELEMENT::MouseInput(click);
}
void ELEMENT_CLOCK::Paint() const
{
	ELEMENT::Paint();

	D2D1_ELLIPSE ellipse;

	RetControlEllipse(ellipse);
	target->FillEllipse(ellipse, state == EL_STATE_TRUE ? brush->Red() : brush->DarkRed());
	target->DrawEllipse(ellipse, brush->Gray());

	RetOutputEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());

	PaintElapse();
	return;
}
void ELEMENT_CLOCK::PaintElapse() const
{
	D2D1_RECT_F rect = D2D1::RectF(pos.x,
								   pos.y+size.height*0.56f,
								   pos.x+size.width,
								   pos.y+size.height);
	wchar_t text[6];		
	_itow_s(elapse, text, 6, 10);
	target->DrawTextA(text, static_cast<unsigned>(wcslen(text)), brush->BigFont(), rect, brush->Black());		
	return;
}

bool ELEMENT_CLOCK::RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const
{
	if (id)
		return false;

	out = RetOutputPoint();
	return true;
}

VOID CALLBACK ClockElementTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	ELEMENT_CLOCK* elc = reinterpret_cast<ELEMENT_CLOCK*>(idEvent);
	elc->state = static_cast<EL_STATE>(!static_cast<int>(elc->state));
	elc->Master->Proceed();
	return;
}

void ELEMENT_CLOCK::Proceed(OUTPUT_LIST* compute_list, unsigned i)
{
	// dodaje do listy wszystkie swoje outputy i ustawia im stany wejœciowe
	for (unsigned i = 0; i < output_list.retAmount(); i++)
	{
		compute_list->add_if_new(output_list[i]->element, output_list[i]->input);
		output_list[i]->element->SetInternalInput(output_list[i]->input, state);
	}

	return;
}
