#pragma once

#include "element.h"

class ELEMENT_OUTPUT : public ELEMENT
{
private:
	EL_STATE state;
	EL_INPUT input;

private:
	ELEMENT_OUTPUT(const ELEMENT_OUTPUT&) {}
	ELEMENT_OUTPUT& operator= (const ELEMENT_OUTPUT&) {}

	ELEMENT_OUTPUT(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height);
	~ELEMENT_OUTPUT() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_OUTPUT* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;
	void PaintWires() const;
	
	bool RetInputPoint(D2D1_POINT_2F& out, unsigned id) const;

	void SetInput(unsigned this_input_id, ELEMENT* target, unsigned input);

	void RemoveLinkage(ELEMENT* target);

	EL_STATE RetState(unsigned output_id = 0) const { return state; }

	void Proceed(unsigned level, unsigned limit);

	friend class ELEMENTS_SET;
};