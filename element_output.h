#pragma once

#include "element.h"
#include "output_list.h"

class ELEMENT_OUTPUT : public ELEMENT
{
private:
	bool state;
	OUTPUT_LIST output_list;

private:
	ELEMENT_OUTPUT(const ELEMENT_OUTPUT&) {}
	ELEMENT_OUTPUT& operator= (const ELEMENT_OUTPUT&) {}

	ELEMENT_OUTPUT(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height);
	~ELEMENT_OUTPUT() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_OUTPUT* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint();

	friend class ELEMENTS_SET;
};