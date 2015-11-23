#pragma once

#include "element.h"

class ELEMENT_CLOCK : public ELEMENT
{
private:
	EL_STATE state;

private:
	ELEMENT_CLOCK(const ELEMENT_CLOCK&) {}
	ELEMENT_CLOCK& operator= (const ELEMENT_CLOCK&) {}

	ELEMENT_CLOCK(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height);
	~ELEMENT_CLOCK() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetOutputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetOutputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_CLOCK* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const;

	friend class ELEMENTS_SET;
};