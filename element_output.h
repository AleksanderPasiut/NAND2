#pragma once

#include "element.h"

class ELEMENT_OUTPUT : public ELEMENT
{
	NONCOPYABLE(ELEMENT_OUTPUT)

private:
	EL_STATE state;
	EL_INPUT input;

private:
	~ELEMENT_OUTPUT() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPortPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputPortEllipse(D2D1_ELLIPSE&) const;

public:
	ELEMENT_OUTPUT(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;
	void PaintWires() const;
	
	bool RetInputPortPoint(D2D1_POINT_2F& out, unsigned id) const;

	void SetInput(unsigned this_input_port_id, ELEMENT* target, unsigned target_output_port_id);

	void RemoveLinkage(ELEMENT* target);

	EL_STATE RetState(unsigned output_port_id = 0) const { return state; }

	bool Proceed(OUTPUT_LIST* sucessing_elements_list, unsigned input_port_id = 0);

	friend class ELEMENTS_SET;
};