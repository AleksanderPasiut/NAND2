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
#include "element_output.h"

class MASTER
{
private:
	HWND hwnd;
	ID2D1Factory* factory;
	IDWriteFactory* dwfactory;
	ID2D1HwndRenderTarget* target;
	BRUSH_SET* brush_set;
	IDWriteTextFormat* text_format;
	ELEMENTS_SET elements_set;
	MENU* menu;

	ELEMENT* element_moved;
	D2D1_POINT_2F old_pos;
	D2D1_POINT_2F click;

private:
	// master_create.cpp
	static D2D1_SIZE_U RetHwndClientSize(HWND); 

private:
	MASTER() {}
	MASTER(const MASTER&) {}
	MASTER& operator= (const MASTER&) {}

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
};