#pragma once

#include "element.h"

class ELEMENT_TEXTBOX : public ELEMENT
{
private:
	static const unsigned int text_size = 3;
	wchar_t text[text_size];
	bool pointer;
	float angle;

private:
	ELEMENT_TEXTBOX(const ELEMENT_TEXTBOX&) {}
	ELEMENT_TEXTBOX& operator= (const ELEMENT_TEXTBOX&) {}

	ELEMENT_TEXTBOX(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float radiusX, float radiusY, bool in_pointer);
	~ELEMENT_TEXTBOX() {}

private:
	void RetBodyEllipse(D2D1_ELLIPSE&) const;
	void RetPointerAreaEllipse(D2D1_ELLIPSE&) const;
	void RetCrossRect(D2D1_RECT_F&) const;
	void PaintText() const;
	void PaintPointer() const;

public:
	static ELEMENT_TEXTBOX* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float radiusX, float radiusY, bool in_pointer, char* text);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;
	void SetPos(D2D1_POINT_2F);

	friend class ELEMENTS_SET;
};