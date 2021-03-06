#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include "noncopyable.h"

class BRUSH_SET
{
	NONCOPYABLE(BRUSH_SET)

private:
	static const unsigned int BRUSH_AMOUNT = 10;
	ID2D1SolidColorBrush* brush[BRUSH_AMOUNT];
	ID2D1StrokeStyle* stroke;
	IDWriteTextFormat* big_font;
	IDWriteTextFormat* normal_font;
	IDWriteTextFormat* small_font;

public:
	BRUSH_SET(ID2D1HwndRenderTarget*, IDWriteFactory*);
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
	ID2D1SolidColorBrush* Green()		const { return brush[8]; }
	ID2D1SolidColorBrush* Orange()		const { return brush[9]; }
	ID2D1StrokeStyle* Stroke()			const { return stroke; }
	IDWriteTextFormat* BigFont()		const { return big_font; }
	IDWriteTextFormat* Font()			const { return normal_font; }
	IDWriteTextFormat* SmallFont()		const { return small_font; }
};