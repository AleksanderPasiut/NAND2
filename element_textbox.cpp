#include <dwrite.h>
#include <math.h>
#include "element_textbox.h"

ELEMENT_TEXTBOX::ELEMENT_TEXTBOX(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   float pos_x,
							   float pos_y,
							   float radiusX,
							   float radiusY,
							   bool in_pointer)
	: ELEMENT(target, brush_set, pos_x, pos_y, radiusX, radiusY)
{
	pointer = in_pointer;
	text[0] = 0;
	text[1] = 0;
	text[2] = 0;
	angle = 0.0f;
}

void ELEMENT_TEXTBOX::RetBodyEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	out = D2D1::Ellipse(D2D1::Point2F(pos.x*ts.width, pos.y*ts.height), size.width*ts.width/2, size.height*ts.height/2);
	return;
}
void ELEMENT_TEXTBOX::RetPointerAreaEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	out = D2D1::Ellipse(D2D1::Point2F(pos.x*ts.width, pos.y*ts.height), 1.5f*size.width*ts.width/2, 1.5f*size.height*ts.height/2);
	return;
}
void ELEMENT_TEXTBOX::RetCrossRect(D2D1_RECT_F& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	static const float ds = 0.045f;
	static const float s = 0.01f;
	float multiplier = min(ts.height, ts.width);
	out.left	= pos.x*ts.width-s*multiplier;
	out.top		= (pos.y-ds)*ts.height;
	out.right	= pos.x*ts.width+s*multiplier;
	out.bottom	= (pos.y-ds)*ts.height+2*s*multiplier;
	return;
}
void ELEMENT_TEXTBOX::PaintText() const
{
	return;
}
void ELEMENT_TEXTBOX::PaintPointer() const
{
	D2D1_SIZE_F ts = target->GetSize();
	D2D1_POINT_2F begin = D2D1::Point2F(pos.x*ts.width,
										pos.y*ts.height);
	D2D1_POINT_2F end = D2D1::Point2F((pos.x+1.2f*size.width*cos(angle))*ts.width,
									  (pos.y+1.2f*size.height*sin(angle))*ts.height);
	target->DrawLine(begin, end, brush->DarkGray(), 2.0f);
	return;
}

ELEMENT_TEXTBOX* ELEMENT_TEXTBOX::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   float pos_x,
									   float pos_y,
									   float radiusX,
									   float radiusY,
									   bool in_pointer,
									   char* text)
{
	ELEMENT_TEXTBOX* ret = new ELEMENT_TEXTBOX(target,
											 brush_set,
											 pos_x,
											 pos_y,
											 radiusX,
											 radiusY,
											 in_pointer);

	if (ret)
	{
		if (text)
			mbstowcs_s(0, ret->text, text, 3);

		return ret;
	}
	
	return 0;
}

EVPV ELEMENT_TEXTBOX::MouseInput(const D2D1_POINT_2F& click)
{
	D2D1_RECT_F rect;
	RetCrossRect(rect);
	if (PointInRect(rect, click))
		return EVPV(EVPV_CROSS);

	D2D1_ELLIPSE ellipse;
	RetBodyEllipse(ellipse);
	if (PointInEllipse(ellipse, click))
		return EVPV(EVPV_BODY);

	return EVPV(EVPV_NONE);
}
void ELEMENT_TEXTBOX::Paint() const
{
	if (pointer)
		PaintPointer();

	D2D1_ELLIPSE ellipse;
	RetBodyEllipse(ellipse);
	target->FillEllipse(ellipse, brush->LightGray());
	target->DrawEllipse(ellipse, brush->Gray());

	D2D1_RECT_F rect;
	RetCrossRect(rect);
	target->FillRectangle(rect, brush->DarkGray());
	target->DrawRectangle(rect, brush->Gray());
	target->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.right, rect.bottom), brush->Gray());
	target->DrawLine(D2D1::Point2F(rect.left, rect.bottom), D2D1::Point2F(rect.right, rect.top), brush->Gray());

	PaintText();

	return;
}
void ELEMENT_TEXTBOX::SetPos(D2D1_POINT_2F new_pos)
{
	if (new_pos.x < 0.0f)
		new_pos.x = 0.0f;
	if (new_pos.x > 1.0f)
		new_pos.x = 1.0f;
	if (new_pos.y < 0.0f)
		new_pos.y = 0.0f;
	if (new_pos.y > 1.0f)
		new_pos.y = 1.0f;

	pos = new_pos;
	return;
}