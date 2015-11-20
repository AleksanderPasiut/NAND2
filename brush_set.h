#pragma once

#include <d2d1.h>

class BRUSH_SET
{
private:
	static const unsigned int BRUSH_AMOUNT = 8;
	ID2D1SolidColorBrush* brush[BRUSH_AMOUNT];

	BRUSH_SET() {}
	BRUSH_SET(const BRUSH_SET&) {}
	BRUSH_SET& operator= (const BRUSH_SET&) {}

public:
	static BRUSH_SET* Create(ID2D1HwndRenderTarget*);
	~BRUSH_SET();

public:
	ID2D1SolidColorBrush* Azure()		const { return brush[0]; }
	ID2D1SolidColorBrush* Black()		const { return brush[1]; }
	ID2D1SolidColorBrush* DarkGray()	const { return brush[2]; }
	ID2D1SolidColorBrush* Gray()		const { return brush[3]; }
	ID2D1SolidColorBrush* LightGray()	const { return brush[4]; }
	ID2D1SolidColorBrush* DarkRed()		const { return brush[5]; }
	ID2D1SolidColorBrush* Red()			const { return brush[6]; }
	ID2D1SolidColorBrush* Yellow()		const { return brush[7]; }
};