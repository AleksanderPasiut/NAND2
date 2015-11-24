#include <dwrite.h>
#include "element_clock.h"
#include "master.h"

ELEMENT_CLOCK::ELEMENT_CLOCK(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   float pos_x,
							   float pos_y,
							   float width,
							   float height,
							   unsigned in_elapse,
							   MASTER* in_Master)
	: ELEMENT(target, brush_set, pos_x, pos_y, width, height)
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
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.3f*size.width),
						 ts.height*(pos.y+0.4f*size.height));
}
D2D1_POINT_2F ELEMENT_CLOCK::RetOutputPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.7f*size.width),
						 ts.height*(pos.y+0.4f*size.height));
}
void ELEMENT_CLOCK::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.02f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetControlPoint(), radius, radius);
	return;
}
void ELEMENT_CLOCK::RetOutputEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.012f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetOutputPoint(), radius, radius);
	return;
}

ELEMENT_CLOCK* ELEMENT_CLOCK::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   float pos_x,
									   float pos_y,
									   float width,
									   float height,
									   unsigned in_elapse,
									   MASTER* Master)
{
	ELEMENT_CLOCK* ret = new ELEMENT_CLOCK(target,
											 brush_set,
											 pos_x,
											 pos_y,
											 width,
											 height,
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