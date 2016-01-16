#pragma once

#include "element.h"

class MASTER;

class ELEMENT_CLOCK : public ELEMENT
{
	NONCOPYABLE(ELEMENT_CLOCK)

private:
	EL_STATE state;
	unsigned elapse;
	MASTER* Master;

	OUTPUT_LIST output_list;

private:
	~ELEMENT_CLOCK();

private:
	D2D1_POINT_2F RetControlPoint() const;
	D2D1_POINT_2F RetOutputPortPoint() const;
	void RetControlEllipse(D2D1_ELLIPSE&) const;
	void RetOutputPortEllipse(D2D1_ELLIPSE&) const;
	void PaintElapse() const;

public:
	ELEMENT_CLOCK(ID2D1HwndRenderTarget*, BRUSH_SET*, float pos_x, float pos_y, unsigned id, unsigned in_elapse, MASTER* Master);

	EVPV MouseInput(const D2D1_POINT_2F&);
	void Paint() const;

	bool RetOutputPortPoint(D2D1_POINT_2F& out, unsigned id) const;

	void AddOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list.add(target, target_input_port_id); return; }
	void DelOutput(unsigned this_output_port_id, ELEMENT* target, unsigned target_input_port_id) { output_list.remove(target, target_input_port_id); return; }
	const OUTPUT_LIST& RetOutputList() const { return output_list; }
	
	void RemoveLinkage(ELEMENT* target) { output_list.remove(target); return; }

	void SetState(EL_STATE state) { this->state = state; }
	EL_STATE RetState(unsigned output_port_id = 0) const { return state; }
	unsigned RetElapse() const { return elapse; }

	bool Proceed(OUTPUT_LIST* sucessing_elements_list, unsigned input_port_id = 0);

	friend class ELEMENTS_SET;
	friend VOID CALLBACK ClockElementTimerProc(HWND, UINT, UINT_PTR, DWORD);
};