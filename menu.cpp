#include <stdexcept>

#include "menu.h"

MENU::MENU(HWND in_hwnd)
	: hwnd(in_hwnd),
	  lParam(0)
{
	try
	{
		hMenu = LoadMenu(0, "res_popup_menu");
		if (!hMenu)
			throw 0;

		hSubMenu = GetSubMenu(hMenu, 0);
		if (!hSubMenu)
			throw 1;
	}
	catch(int error_code)
	{
		switch(error_code)
		{
		case 1: DestroyMenu(hMenu);
		case 0: throw std::bad_alloc();
		}
	}
}
MENU::~MENU()
{
	DestroyMenu(hSubMenu);
	DestroyMenu(hMenu);
}

void MENU::Show(LPARAM lParam)
{
	this->lParam = lParam;
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	if (!ClientToScreen(hwnd, &pt))
		return;
	TrackPopupMenu(hSubMenu, 0, pt.x, pt.y, 0, hwnd, 0);
	return;
}