#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include "brush_set.h"
#include "output_list.h"
#include "evpv.h"

class ELEMENT
{
protected:
	ID2D1HwndRenderTarget* target;
	BRUSH_SET* brush;

	D2D1_POINT_2F pos;
	D2D1_SIZE_F size;

	unsigned id;
	bool source_flag;

public:
	static const float CROSS_SIZE;
	static const float CROSS_MARGIN;
	static const float PORT_RADIUS;
	static const float PORT_MARK_RADIUS;
	static const float CONTROL_RADIUS;

protected:
	// konstruktory chronione - tworzenie odbywa siê poprzez Create() klas pochodnych
	ELEMENT() {}
	ELEMENT(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height, unsigned id, bool source_flag);

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
	void PaintId() const;

public:
	virtual EVPV MouseInput(const D2D1_POINT_2F&);
	virtual void Paint() const;
	virtual void PaintWires() const {};
	void SetPos(D2D1_POINT_2F);
	D2D1_POINT_2F RetPos() const { return pos; }
	bool RetSourceFlag() const { return source_flag; }
	unsigned RetId() const { return id; }

	virtual bool RetInputPoint(D2D1_POINT_2F& out, unsigned id) const { return false; }
	virtual bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const { return false; }

	virtual void SetInput(unsigned this_input_id, ELEMENT* target, unsigned input) {}

	virtual void AddOutput(unsigned this_output_id, ELEMENT* target, unsigned input) {}
	virtual void DelOutput(unsigned this_output_id, ELEMENT* target, unsigned input) {}
	virtual void RemoveLinkage(ELEMENT* target) {}

	virtual EL_STATE RetState(unsigned output_id = 0) const { return EL_STATE_FALSE; }

	virtual void Reset() {}
	virtual bool UpdateState(unsigned i = 0) { return true; }
	virtual void Proceed(OUTPUT_LIST*, unsigned input = 0) {}
	virtual void SetInternalInput(unsigned input, EL_STATE state) {}

	friend class ELEMENTS_SET;
};