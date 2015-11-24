#include <dwrite.h>
#include "element_textbox.h"

ELEMENT_TEXTBOX::ELEMENT_TEXTBOX(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   IDWriteTextFormat* text_format,
							   float pos_x,
							   float pos_y,
							   float radiusX,
							   float radiusY,
							   bool in_pointer)
	: ELEMENT(target, brush_set, text_format, pos_x, pos_y, radiusX, radiusY)
{
	pointer = in_pointer;
	text[0] = 0;
	text[1] = 0;
	text[2] = 0;
}

void ELEMENT_TEXTBOX::RetBodyEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	out = D2D1::Ellipse(D2D1::Point2F(pos.x*ts.width, pos.y*ts.height), size.width*ts.width/2, size.height*ts.height/2);
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
	D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, 3.0f, 1.f);
	D2D1_SIZE_F ts = target->GetSize();
	float scale = min(ts.width*size.width/3.0f, ts.height*size.height*0.4f);
	target->SetTransform(D2D1::Matrix3x2F::Scale(scale, scale, D2D1::Point2F())*
						 D2D1::Matrix3x2F::Translation(D2D1::SizeF(ts.width*(pos.x-size.width/2)+(ts.width*size.width/3.0f-scale)*1.5f,
																   ts.height*(pos.y-0.1f*size.height))));

	text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	target->DrawTextA(text, static_cast<unsigned int>(wcslen(text)), text_format, rect, brush->Black());
	target->SetTransform(D2D1::IdentityMatrix());
	return;
}

ELEMENT_TEXTBOX* ELEMENT_TEXTBOX::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   IDWriteTextFormat* text_format,
									   float pos_x,
									   float pos_y,
									   float radiusX,
									   float radiusY,
									   bool in_pointer,
									   char* text)
{
	ELEMENT_TEXTBOX* ret = new ELEMENT_TEXTBOX(target,
											 brush_set,
											 text_format,
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