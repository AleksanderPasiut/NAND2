#pragma once

#include "element.h"
#include "output_list.h"

class ELEMENT_NAND : public ELEMENT
{
private:
	unsigned ia;
	bool state;
	OUTPUT_LIST output_list;

private:
	ELEMENT_NAND(const ELEMENT_NAND&) {}
	ELEMENT_NAND& operator= (const ELEMENT_NAND&) {}

	ELEMENT_NAND(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height, unsigned input_amount);
	~ELEMENT_NAND() {}

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetInputPoint(unsigned) const;
	D2D1_POINT_2F RetOutputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetInputEllipse(D2D1_ELLIPSE&, unsigned) const;
	void RetOutputEllipse(D2D1_ELLIPSE&) const;

public:
	static ELEMENT_NAND* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height, unsigned input_amount);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint();

	friend class ELEMENTS_SET;
};