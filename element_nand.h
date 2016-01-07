#pragma once

#include "element.h"

class ELEMENT_NAND : public ELEMENT
{
public:
	static const unsigned max_input = 10;

private:
	unsigned ia;
	EL_STATE state;
	EL_INPUT input[max_input];

	OUTPUT_LIST output_list;

private:
	ELEMENT_NAND(const ELEMENT_NAND&) {}
	ELEMENT_NAND& operator= (const ELEMENT_NAND&) {}

	ELEMENT_NAND(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id, unsigned input_amount);
	~ELEMENT_NAND() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPoint(unsigned) const;
	D2D1_POINT_2F RetOutputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputEllipse(D2D1_ELLIPSE&, unsigned) const;
	void RetOutputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_NAND* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id, unsigned input_amount);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	void PaintWires() const;
	bool RetInputPoint(D2D1_POINT_2F& out, unsigned id) const;
	bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const;

	void SetInput(unsigned this_input_id, ELEMENT* target, unsigned input);
	unsigned RetInputAmount() const { return ia; }

	void AddOutput(unsigned this_output_id, ELEMENT* target, unsigned input) { output_list.add(target, input); return; }
	void DelOutput(unsigned this_output_id, ELEMENT* target, unsigned input) { output_list.remove(target, input); return; }
	const OUTPUT_LIST& RetOutputList() const { return output_list; }

	void RemoveLinkage(ELEMENT* target);

	void SetState(EL_STATE state) { this->state = state; }
	EL_STATE RetState(unsigned output_id = 0) const { return state; }

	bool Proceed(bool);

	friend class ELEMENTS_SET;
};