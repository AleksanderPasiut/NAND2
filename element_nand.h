#pragma once

#include "element.h"

class ELEMENT_NAND : public ELEMENT
{
	NONCOPYABLE(ELEMENT_NAND)

public:
	static const unsigned max_input = 10;

private:
	unsigned ia;
	EL_STATE state;
	EL_INPUT input[max_input];

	OUTPUT_LIST output_list;

	bool propagation;

private:
	ELEMENT_NAND(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id, unsigned input_amount);
	~ELEMENT_NAND() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPortPoint(unsigned) const;
	D2D1_POINT_2F RetOutputPortPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputPortEllipse(D2D1_ELLIPSE&, unsigned) const;
	void RetOutputPortEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_NAND* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id, unsigned input_amount);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	void PaintWires() const;
	bool RetInputPortPoint(D2D1_POINT_2F& out, unsigned port_id) const;
	bool RetOutputPortPoint(D2D1_POINT_2F& out, unsigned port_id) const;

	void SetInput(unsigned this_input_port_id, ELEMENT* target, unsigned target_output_port_id);
	unsigned RetInputAmount() const { return ia; }

	void AddOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list.add(target, target_input_port_id); return; }
	void DelOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list.remove(target, target_input_port_id); return; }
	const OUTPUT_LIST& RetOutputList() const { return output_list; }

	void RemoveLinkage(ELEMENT* target);

	void SetState(EL_STATE state) { this->state = state; }
	EL_STATE RetState(unsigned output_id = 0) const { return state; }

	bool Proceed(OUTPUT_LIST* sucessing_elements_list, unsigned input_port_id = 0);

	friend class ELEMENTS_SET;
};