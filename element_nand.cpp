#include "element_nand.h"

ELEMENT_NAND::ELEMENT_NAND(ID2D1HwndRenderTarget* target,
							   BRUSH_SET* brush_set,
							   IDWriteTextFormat* text_format,
							   float pos_x,
							   float pos_y,
							   float width,
							   float height,
							   unsigned input_amount)
	: ELEMENT(target, brush_set, text_format, pos_x, pos_y, width, height)
{
	ia = input_amount;
	state = false;
}

D2D1_POINT_2F ELEMENT_NAND::RetControlPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.5f*size.width),
						 ts.height*(pos.y+size.height/2));
}
D2D1_POINT_2F ELEMENT_NAND::RetInputPoint(unsigned i) const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.16f*size.width),
						 ts.height*(pos.y+size.height*static_cast<float>(i+1)/static_cast<float>(ia+1)));
}
D2D1_POINT_2F ELEMENT_NAND::RetOutputPoint() const
{
	D2D1_SIZE_F ts = target->GetSize();
	return D2D1::Point2F(ts.width*(pos.x+0.84f*size.width),
						 ts.height*(pos.y+size.height/2));
}
void ELEMENT_NAND::RetControlEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.02f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetControlPoint(), radius, radius);
	return;
}
void ELEMENT_NAND::RetInputEllipse(D2D1_ELLIPSE& out, unsigned i) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.012f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetInputPoint(i), radius, radius);
	return;
}
void ELEMENT_NAND::RetOutputEllipse(D2D1_ELLIPSE& out) const
{
	D2D1_SIZE_F ts = target->GetSize();
	float radius = 0.012f*min(ts.height, ts.width);
	out = D2D1::Ellipse(RetOutputPoint(), radius, radius);
	return;
}

ELEMENT_NAND* ELEMENT_NAND::Create(ID2D1HwndRenderTarget* target,
									   BRUSH_SET* brush_set,
									   IDWriteTextFormat* text_format,
									   float pos_x,
									   float pos_y,
									   float width,
									   float height,
									   unsigned input_amount)
{
	ELEMENT_NAND* ret = new ELEMENT_NAND(target,
											 brush_set,
											 text_format,
											 pos_x,
											 pos_y,
											 width,
											 height,
											 input_amount);

	if (ret)
	{
		return ret;
	}
	
	return 0;
}

EVPV ELEMENT_NAND::MouseInput(const D2D1_POINT_2F& click)
{
	return ELEMENT::MouseInput(click);
}
void ELEMENT_NAND::Paint()
{
	ELEMENT::Paint();

	D2D1_ELLIPSE ellipse;

	RetControlEllipse(ellipse);
	target->FillEllipse(ellipse, state ? brush->Red() : brush->DarkRed());
	target->DrawEllipse(ellipse, brush->Gray());

	for (unsigned i = 0; i < ia; i++)
	{
		RetInputEllipse(ellipse, i);
		target->FillEllipse(ellipse, brush->Black());
		target->DrawEllipse(ellipse, brush->Gray());
	}

	RetOutputEllipse(ellipse);
	target->FillEllipse(ellipse, brush->Black());
	target->DrawEllipse(ellipse, brush->Gray());
	return;
}