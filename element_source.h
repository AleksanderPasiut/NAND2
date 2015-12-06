#pragma once

#include "element.h"

class ELEMENT_SOURCE : public ELEMENT
{
private:
	EL_STATE state;

	OUTPUT_LIST output_list;

private:
	ELEMENT_SOURCE(const ELEMENT_SOURCE&) {}
	ELEMENT_SOURCE& operator= (const ELEMENT_SOURCE&) {}

	ELEMENT_SOURCE(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id);
	~ELEMENT_SOURCE() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetOutputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetOutputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_SOURCE* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const;

	void AddOutput(unsigned this_output_id, ELEMENT* target, unsigned input) { output_list.add(target, input); return; }
	void DelOutput(unsigned this_output_id, ELEMENT* target, unsigned input) { output_list.remove(target, input); return; }
	
	void RemoveLinkage(ELEMENT* target) { output_list.remove(target); return; }

	EL_STATE RetState(unsigned output_id = 0) const { return state; }

	void Proceed(OUTPUT_LIST&);

	friend class ELEMENTS_SET;
};