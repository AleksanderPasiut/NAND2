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

	// pozycja i rozmiar wyra¿one poprzez u³amek okna (od 0.0 do 1.0)
	D2D1_POINT_2F pos;
	D2D1_SIZE_F size;

protected:
	// konstruktory chronione - tworzenie odbywa siê poprzez Create() klas pochodnych
	ELEMENT() {}
	ELEMENT(ID2D1HwndRenderTarget*, BRUSH_SET*, IDWriteTextFormat*, float pos_x, float pos_y, float width, float height);

	// destruktor chroniony - usuwanie odbywa siê poprzez ELEMENTS_SET::remove(ELEMENT*)
	~ELEMENT() {}

private:
	ELEMENT(const ELEMENT&) {}
	ELEMENT* operator= (const ELEMENT&) {}

protected:
	static bool PointInRect(const D2D1_RECT_F&, const D2D1_POINT_2F&);
	static bool PointInEllipse(const D2D1_ELLIPSE&, const D2D1_POINT_2F&);
	void RetElementRect(D2D1_RECT_F&) const;
	virtual void RetCrossRect(D2D1_RECT_F&) const;

public:
	virtual EVPV MouseInput(const D2D1_POINT_2F&);
	virtual void Paint() const;
	virtual void PaintWires() const {};
	virtual void SetPos(D2D1_POINT_2F);
	D2D1_POINT_2F RetPos() const { return pos; }

	virtual void SetInput(ELEMENT* target, unsigned target_id, unsigned input_id) {}
	virtual void RemoveInput(ELEMENT* target) {}
	virtual bool RetInputPoint(D2D1_POINT_2F& out, unsigned id) const { return false; }
	virtual bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const { return false; }

	friend class ELEMENTS_SET;
};