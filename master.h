#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

#include "brush_set.h"
#include "elements_set.h"
#include "menu.h"

#include "element_source.h"
#include "element_nand.h"
#include "element_textbox.h"
#include "element_clock.h"
#include "element_output.h"

class MASTER
{
private:
	HWND hwnd;
	ID2D1Factory* factory;
	IDWriteFactory* dwfactory;
	ID2D1HwndRenderTarget* target;
	BRUSH_SET* brush_set;
	ELEMENTS_SET elements_set;
	MENU* menu;

	struct MOVING
	{
		ELEMENT* element;
		D2D1_POINT_2F old_pos;
		D2D1_POINT_2F click;

		MOVING() : element(0), old_pos(D2D1::Point2F()), click(D2D1::Point2F()) {}
	};
	MOVING moving;

	struct LINKING
	{
		EVPV_TYPE type;
		ELEMENT* element;
		unsigned id;

		bool mark_all_inputs;
		bool mark_all_outputs;

		LINKING() : type(EVPV_NONE), element(0), id(0), mark_all_inputs(false), mark_all_outputs(false) {}
	};
	LINKING linking;

private:
	// master_create.cpp
	static D2D1_SIZE_U RetHwndClientSize(HWND); 

private:
	MASTER() {}
	MASTER(const MASTER&) {}
	MASTER& operator= (const MASTER&) {}

private:
	// master_link.cpp
	void Link(ELEMENT*, EVPV);
	void PaintInputs();
	void PaintOutputs();
	void RemoveElement(ELEMENT*);

public:
	// master_create.cpp
	static MASTER* Create(HWND);
	~MASTER();

	// master_gui.cpp
	void MouseInput(UINT, WPARAM, LPARAM);
	void Paint();
	void Size(WPARAM, LPARAM);
	ELEMENT* Nand(unsigned);
	void MenuInput(WPARAM, LPARAM);

	// master_proceed.cpp
	void Proceed();
};