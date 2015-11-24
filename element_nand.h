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

private:
	ELEMENT_NAND(const ELEMENT_NAND&) {}
	ELEMENT_NAND& operator= (const ELEMENT_NAND&) {}

	ELEMENT_NAND(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height, unsigned input_amount);
	~ELEMENT_NAND() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPoint(unsigned) const;
	D2D1_POINT_2F RetOutputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputEllipse(D2D1_ELLIPSE&, unsigned) const;
	void RetOutputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_NAND* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height, unsigned input_amount);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;
	void PaintWires() const;

	void SetInput(ELEMENT* target, unsigned target_id, unsigned input_id);
	void RemoveInput(ELEMENT* target);
	bool RetInputPoint(D2D1_POINT_2F& out, unsigned id) const;
	bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const;

	friend class ELEMENTS_SET;
};