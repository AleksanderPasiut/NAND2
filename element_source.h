#pragma once

#include "element.h"

class ELEMENT_SOURCE : public ELEMENT
{
	NONCOPYABLE(ELEMENT_SOURCE)

private:
	EL_STATE state;

	OUTPUT_LIST output_list;

private:
	~ELEMENT_SOURCE() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetOutputPortPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetOutputPortEllipse(D2D1_ELLIPSE&) const;

public:
	ELEMENT_SOURCE(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	bool RetOutputPortPoint(D2D1_POINT_2F& out, unsigned port_id) const;

	void AddOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list.add(target, target_input_port_id); return; }
	void DelOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list.remove(target, target_input_port_id); return; }
	const OUTPUT_LIST& RetOutputList() const { return output_list; }
	
	void RemoveLinkage(ELEMENT* target) { output_list.remove(target); return; }

	void SetState(EL_STATE state) { this->state = state; }
	EL_STATE RetState(unsigned output_port_id = 0) const { return state; }

	bool Proceed(OUTPUT_LIST* sucessing_elements_list);

	friend class ELEMENTS_SET;
};