#pragma once

#include <d2d1.h>

class BRUSH_SET
{
private:
	bool ok;

private:
	static const unsigned int BRUSH_AMOUNT = 8;
	ID2D1SolidColorBrush* brush[BRUSH_AMOUNT];

	BRUSH_SET() : ok(false) {}
	BRUSH_SET(const BRUSH_SET&) {}
	BRUSH_SET& operator= (const BRUSH_SET&) {}

public:
	static BRUSH_SET* Create(ID2D1HwndRenderTarget*);
	~BRUSH_SET();

public:
	const ID2D1SolidColorBrush* Azure()		const { return brush[0]; }
	const ID2D1SolidColorBrush* Black()		const { return brush[1]; }
	const ID2D1SolidColorBrush* DarkGray()  const { return brush[2]; }
	const ID2D1SolidColorBrush* Gray()		const { return brush[3]; }
	const ID2D1SolidColorBrush* LightGray() const { return brush[4]; }
	const ID2D1SolidColorBrush* DarkRed()	const { return brush[5]; }
	const ID2D1SolidColorBrush* Red()		const { return brush[6]; }
	const ID2D1SolidColorBrush* Yellow()	const { return brush[7]; }
};