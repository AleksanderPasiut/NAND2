#pragma once

#include <d2d1.h>

#include "brush_set.h"
#include "output_list.h"
#include "evpv.h"

class ELEMENT
{
protected:
	// render_target i zestaw pêdzli
	ID2D1HwndRenderTarget* target;
	BRUSH_SET* brush;

	// pozycja i rozmiar wyra¿one poprzez u³amek okna (od 0.0 do 1.0)
	D2D1_POINT_2F pos;
	D2D1_SIZE_F size;

	// flaga Ÿród³a
	bool source_flag;

protected:
	// konstruktory chronione - tworzenie odbywa siê poprzez Create() klas pochodnych
	ELEMENT() {}
	ELEMENT(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height, bool source_flag);

	// destruktor chroniony - usuwanie odbywa siê poprzez ELEMENTS_SET::remove(ELEMENT*)
	~ELEMENT() {}

private:
	ELEMENT(const ELEMENT&) {}
	ELEMENT* operator= (const ELEMENT&) {}

protected:
	static bool PointInRect(const D2D1_RECT_F&, const D2D1_POINT_2F&);
	static bool PointInEllipse(const D2D1_ELLIPSE&, const D2D1_POINT_2F&);
	void RetElementRect(D2D1_RECT_F&) const;
	void RetCrossRect(D2D1_RECT_F&) const;

public:
	virtual EVPV MouseInput(const D2D1_POINT_2F&);
	virtual void Paint() const;
	virtual void PaintWires() const {};
	void SetPos(D2D1_POINT_2F);
	D2D1_POINT_2F RetPos() const { return pos; }
	bool RetSourceFlag() const { return source_flag; }

	virtual bool RetInputPoint(D2D1_POINT_2F& out, unsigned id) const { return false; }
	virtual bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const { return false; }

	virtual void SetInput(unsigned this_input_id, ELEMENT* target, unsigned input) {}

	virtual void AddOutput(unsigned this_output_id, ELEMENT* target, unsigned input) {}
	virtual void DelOutput(unsigned this_output_id, ELEMENT* target, unsigned input) {}
	virtual void RemoveLinkage(ELEMENT* target) {}

	virtual EL_STATE RetState(unsigned output_id = 0) const { return EL_STATE_FALSE; }

	virtual void Proceed(OUTPUT_LIST&) {}

	friend class ELEMENTS_SET;
};