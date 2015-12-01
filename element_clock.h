#pragma once

#include "element.h"

class MASTER;

class ELEMENT_CLOCK : public ELEMENT
{
private:
	EL_STATE state;
	unsigned elapse;
	MASTER* Master;
	IDWriteTextFormat* text_format;

	OUTPUT_LIST output_list;

private:
	ELEMENT_CLOCK(const ELEMENT_CLOCK&) {}
	ELEMENT_CLOCK& operator= (const ELEMENT_CLOCK&) {}

	ELEMENT_CLOCK(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height, unsigned in_elapse, MASTER* Master, IDWriteTextFormat* text_format);
	~ELEMENT_CLOCK();

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetOutputPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetOutputEllipse(D2D1_ELLIPSE&) const;
	void PaintElapse() const;

public:
	static ELEMENT_CLOCK* Create(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, float width, float height, unsigned in_elapse, MASTER* Master, IDWriteTextFormat* text_format);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	bool RetOutputPoint(D2D1_POINT_2F& out, unsigned id) const;

	void AddOutput(unsigned this_output_id, ELEMENT* target, unsigned input) { output_list.add(target, input); return; }
	void DelOutput(unsigned this_output_id, ELEMENT* target, unsigned input) { output_list.remove(target, input); return; }
	
	void RemoveLinkage(ELEMENT* target) { output_list.remove(target); return; }

	EL_STATE RetState(unsigned output_id = 0) const { return state; }

	void Proceed(unsigned level, unsigned limit);

	friend class ELEMENTS_SET;
	friend VOID CALLBACK ClockElementTimerProc(HWND, UINT, UINT_PTR, DWORD);
};