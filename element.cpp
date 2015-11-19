#include "element.h"

ELEMENT::ELEMENT(ID2D1HwndRenderTarget* in_target,
				 BRUSH_SET* brush_set,
				 IDWriteTextFormat* in_text_format,
				 float pos_x,
				 float pos_y,
				 float width,
				 float height) :
	target(in_target),
	brush(brush_set),
	text_format(in_text_format),
	pos(D2D1::Point2F(pos_x, pos_y)),
	size(D2D1::SizeF(width, height)) {}

ELEMENT::~ELEMENT() {}

EVPV ELEMENT::MouseInput(const D2D1_POINT_2F& click)
{
	if (MouseOn(click))
		return EVPV(EVPV_BODY);

	return EVPV(EVPV_NONE);
}
void ELEMENT::Paint()
{
	D2D1_SIZE_F ts = target->GetSize();
	const D2D1_RECT_F rect = D2D1::RectF(pos.x*ts.width, 
										 pos.y*ts.height,
										 (pos.x+size.width)*ts.width,
										 (pos.y+size.height)*ts.height);

	target->FillRectangle(&rect, brush->Gray());
	target->DrawRectangle(&rect, brush->DarkGray());
	return;
}
void ELEMENT::SetPos(const D2D1_POINT_2F& new_pos)
{
	pos = new_pos;

	Paint();
	return;
}
bool ELEMENT::MouseOn(const D2D1_POINT_2F& pt) const
{
	D2D1_SIZE_F ts = target->GetSize();
	if (pt.x < pos.x*ts.width ||
		pt.y < pos.y*ts.height ||
		pt.x > (pos.x+size.width)*ts.width,
		pt.y > (pos.y+size.height)*ts.height)
		return false;
	else return true;
}