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

bool ELEMENT::PointInRect(const D2D1_RECT_F& rect, const D2D1_POINT_2F& pt)
{
	if (pt.x < rect.left || rect.right < pt.x || pt.y < rect.top || rect.bottom < pt.y)
		return false;
	else return true;
}
bool ELEMENT::PointInCircle(const D2D1_ELLIPSE& circle, const D2D1_POINT_2F& pt)
{
	if ((pt.x-circle.point.x)*(pt.x-circle.point.x)+(pt.y-circle.point.y)*(pt.y-circle.point.y) <= circle.radiusX*circle.radiusY)
		return true;
	else return false;
}
void ELEMENT::RetElementRect(D2D1_RECT_F& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	out.left	= pos.x*ts.width;
	out.top		= pos.y*ts.height;
	out.right	= (pos.x+size.width)*ts.width;
	out.bottom	= (pos.y+size.height)*ts.height;
	return;
}
void ELEMENT::RetCrossRect(D2D1_RECT_F& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	static const float ds = 0.035f;
	static const float s = 0.01f;
	float multiplier = min(ts.height, ts.width);
	out.left	= (pos.x+size.width)*ts.width-ds*multiplier;
	out.top		= (pos.y)*ts.height+s*multiplier;
	out.right	= (pos.x+size.width)*ts.width-s*multiplier;
	out.bottom	= (pos.y)*ts.height+ds*multiplier;
	return;
}

EVPV ELEMENT::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_RECT_F rect;

	RetCrossRect(rect);
	if (PointInRect(rect, click))
		return EVPV(EVPV_CROSS);

	RetElementRect(rect);
	if (PointInRect(rect, click))
		return EVPV(EVPV_BODY);

	return EVPV(EVPV_NONE);
}
void ELEMENT::Paint()
{
	D2D1_RECT_F rect;
	RetElementRect(rect);
	target->FillRectangle(rect, brush->LightGray());
	target->DrawRectangle(rect, brush->Gray());

	RetCrossRect(rect);
	target->FillRectangle(rect, brush->DarkGray());
	target->DrawRectangle(rect, brush->Gray());
	target->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.right, rect.bottom), brush->Gray());
	target->DrawLine(D2D1::Point2F(rect.left, rect.bottom), D2D1::Point2F(rect.right, rect.top), brush->Gray());

	return;
}
void ELEMENT::SetPos(D2D1_POINT_2F new_pos)
{
	// ogranicznik przed usuniêciem poza ekran
	if (new_pos.x < -size.width/2)
		new_pos.x = -size.width/2;
	else if (1.0f-size.width/2 < new_pos.x)
		new_pos.x = 1.0f-size.width/2;

	if (new_pos.y < -size.height/2)
		new_pos.y = -size.height/2;
	else if (1.0f-size.height/2 < new_pos.y)
		new_pos.y = 1.0f-size.height/2;

	pos = new_pos;
	return;
}
