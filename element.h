#pragma once

#include <d2d1.h>

#include "brush_set.h"
#include "evpv.h"

class ELEMENT
{
protected:
	ID2D1HwndRenderTarget* target;
	BRUSH_SET* brush;
	IDWriteTextFormat* text_format;

	// pozycja i rozmiar wyra�one poprzez u�amek okna (od 0.0 do 1.0)
	D2D1_POINT_2F pos;
	D2D1_SIZE_F size;

protected:
	// konstruktor chroniony - tworzenie odbywa si� poprzez Create() klas pochodnych
	ELEMENT(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height);

	// destruktor chroniony - usuwanie odbywa si� poprzez ELEMENTS_SET::remove(ELEMENT*)
	virtual ~ELEMENT();

private:
	ELEMENT(const ELEMENT&) {}
	ELEMENT* operator= (const ELEMENT&) {}

protected:
	static bool PointInRect(const D2D1_RECT_F&, const D2D1_POINT_2F&);
	static bool PointInCircle(const D2D1_ELLIPSE&, const D2D1_POINT_2F&);
	void RetElementRect(D2D1_RECT_F&) const;
	void RetCrossRect(D2D1_RECT_F&) const;

public:
	virtual EVPV MouseInput(const D2D1_POINT_2F&);
	virtual void Paint();
	void SetPos(D2D1_POINT_2F);
	D2D1_POINT_2F RetPos() const { return pos; }

	friend class ELEMENTS_SET;
	friend class MASTER; // TEMP
};