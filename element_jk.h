#pragma once
#pragma once

#include "element.h"

class ELEMENT_JK : public ELEMENT
{
	NONCOPYABLE(ELEMENT_JK)

private:
	EL_STATE state;
	EL_INPUT input[5];

	OUTPUT_LIST output_list[2];

	bool propagation;

private:
	~ELEMENT_JK() {}

private:
	D2D1_POINT_2F RetControlPoint(unsigned) const;
	D2D1_POINT_2F RetInputPortPoint(unsigned) const;
	D2D1_POINT_2F RetOutputPortPoint(unsigned) const;
	void RetControlEllipse(D2D1_ELLIPSE&, unsigned) const;
	void RetInputPortEllipse(D2D1_ELLIPSE&, unsigned) const;
	void RetOutputPortEllipse(D2D1_ELLIPSE&, unsigned) const;
	void DrawTexts() const;

public:
	ELEMENT_JK(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	void PaintWires() const;
	bool RetInputPortPoint(D2D1_POINT_2F& out, unsigned port_id) const;
	bool RetOutputPortPoint(D2D1_POINT_2F& out, unsigned port_id) const;

	void SetInput(unsigned this_input_port_id, ELEMENT* target, unsigned target_output_port_id);

	void AddOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list[this_output_port_id].add(target, target_input_port_id); return; }
	void DelOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list[this_output_port_id].remove(target, target_input_port_id); return; }
	const OUTPUT_LIST& RetOutputList(unsigned output_port_id) const { return output_list[output_port_id]; }

	void RemoveLinkage(ELEMENT* target);

	void SetState(EL_STATE state) { this->state = state; }
	EL_STATE RetState(unsigned output_id) const;

	bool Proceed(OUTPUT_LIST* sucessing_elements_list);

	friend class ELEMENTS_SET;
};