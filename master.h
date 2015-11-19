#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

#include "brush_set.h"
#include "elements_set.h"
#include "menu.h"

class MASTER
{
private:
	bool ok;

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

private:
	// master_create.cpp
	static D2D1_SIZE_U RetHwndClientSize(HWND); 

private:
	MASTER() : ok(false) {}
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
	void MenuInput(WPARAM, LPARAM);
};