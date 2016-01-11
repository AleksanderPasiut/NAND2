#pragma once

#include "element.h"

class ELEMENT_COMMENT : public ELEMENT
{
	NONCOPYABLE(ELEMENT_COMMENT)

private:
	wchar_t* text;
	IDWriteTextLayout* text_layout;
	static const float TEXT_MARGIN_TOP;
	static const float TEXT_MARGIN;

public:
	ELEMENT_COMMENT(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height);
	~ELEMENT_COMMENT();

public:
	static ELEMENT_COMMENT* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, const char* text, IDWriteFactory* dwfactory);
	static ELEMENT_COMMENT* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, const wchar_t* wtext, IDWriteFactory* dwfactory);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;
	const wchar_t* RetText() const { return text; }

	friend class ELEMENTS_SET;
};