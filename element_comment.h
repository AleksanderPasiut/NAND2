#pragma once

#include "element.h"

class ELEMENT_COMMENT : public ELEMENT
{
private:
	IDWriteTextLayout* text_layout;
	static const float TEXT_MARGIN_TOP;
	static const float TEXT_MARGIN;

public:
	ELEMENT_COMMENT(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height);
	~ELEMENT_COMMENT();

public:
	static ELEMENT_COMMENT* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, const char* text, IDWriteFactory* dwfactory);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	friend class ELEMENTS_SET;
};