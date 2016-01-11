#include <math.h>
#include "element.h"

const float ELEMENT::CROSS_SIZE = 11.0f;
const float ELEMENT::CROSS_MARGIN = 3.0f;
const float ELEMENT::CONTROL_RADIUS = 10.0f;
const float ELEMENT::PORT_RADIUS = 6.5f;
const float ELEMENT::PORT_MARK_RADIUS = 9.0f;

ELEMENT::ELEMENT(ID2D1HwndRenderTarget* in_target,
				 BRUSH_SET* brush_set,
				 float pos_x,
				 float pos_y,
				 float width,
				 float height,
				 unsigned element_id,
				 bool source_flag_in) :
	target(in_target),
	brush(brush_set),
	pos(D2D1::Point2F(pos_x, pos_y)),
	size(D2D1::SizeF(width, height)),
	id(element_id),
	source_flag(source_flag_in) {}

bool ELEMENT::IsPointInRect(const D2D1_RECT_F& rect, const D2D1_POINT_2F& pt)
{
	if (pt.x < rect.left || rect.right < pt.x || pt.y < rect.top || rect.bottom < pt.y)
		return false;
	else return true;
}
bool ELEMENT::IsPointInEllipse(const D2D1_ELLIPSE& ellipse, const D2D1_POINT_2F& pt)
{
	const float a = min(ellipse.radiusX, ellipse.radiusY); // pó³oœ ma³a elipsy
	const float b = max(ellipse.radiusX, ellipse.radiusY); // pó³oœ wielka elipsy
	const double c = 2*b; // suma odleg³oœci od ognisk elipsy (sta³a elipsy)
	const double u = sqrt(b*b-a*a); // odleg³oœæ ogniska od œrodka elipsy
	const double c1 = ellipse.radiusX > ellipse.radiusY ? sqrt((ellipse.point.x-u-pt.x)*(ellipse.point.x-u-pt.x)+(ellipse.point.y-pt.y)*(ellipse.point.y-pt.y))
													   : sqrt((ellipse.point.y-u-pt.y)*(ellipse.point.y-u-pt.y)+(ellipse.point.x-pt.x)*(ellipse.point.x-pt.x));
	const double c2 = ellipse.radiusX > ellipse.radiusY ? sqrt((ellipse.point.x+u-pt.x)*(ellipse.point.x+u-pt.x)+(ellipse.point.y-pt.y)*(ellipse.point.y-pt.y))
													   : sqrt((ellipse.point.y+u-pt.y)*(ellipse.point.y+u-pt.y)+(ellipse.point.x-pt.x)*(ellipse.point.x-pt.x));
	if (c1+c2 <= c)
		return true;
	else return false;
}
void ELEMENT::RetElementRect(D2D1_RECT_F& out) const
{
	out.left	= pos.x;
	out.top		= pos.y;
	out.right	= pos.x+size.width;
	out.bottom	= pos.y+size.height;
	return;
}
void ELEMENT::RetCrossRect(D2D1_RECT_F& out) const
{
	out.left	= pos.x+size.width-CROSS_MARGIN-CROSS_SIZE;
	out.top		= pos.y+CROSS_MARGIN;
	out.right	= pos.x+size.width-CROSS_MARGIN;
	out.bottom	= pos.y+CROSS_MARGIN+CROSS_SIZE;
	return;
}
void ELEMENT::PaintId() const
{
	static const float tw = 60.0f;
	static const float th = 12.0f;
	static const float thp = 2.0f;
	D2D1_RECT_F rect = D2D1::RectF(pos.x+size.width-CROSS_MARGIN*2-CROSS_SIZE-tw,
								   pos.y+CROSS_MARGIN-thp,
								   pos.x+size.width-CROSS_MARGIN*2-CROSS_SIZE,
								   pos.y+CROSS_MARGIN+th-thp);
	wchar_t text[8];		
	_itow_s(id, text, 8, 10);
	target->DrawTextA(text, static_cast<unsigned>(wcslen(text)), brush->SmallFont(), rect, brush->Black());
	return;
}

EVPV ELEMENT::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_RECT_F rect;

	RetCrossRect(rect);
	if (IsPointInRect(rect, click))
		return EVPV(EVPV_CROSS);

	RetElementRect(rect);
	if (IsPointInRect(rect, click))
		return EVPV(EVPV_BODY);

	return EVPV(EVPV_NONE);
}
void ELEMENT::Paint() const
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

	PaintId();
	return;
}
void ELEMENT::SetPos(D2D1_POINT_2F new_pos)
{
	pos = new_pos;
	return;
}
