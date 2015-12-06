#include "menu.h"

MENU* MENU::Create(HWND hwnd)
{
	MENU* ret = new MENU;

	if (!ret)
		return 0;

	if (!(ret->hMenu = LoadMenu(0, "res_popup_menu")))
	{	delete ret;
		return 0;	}

	if (!(ret->hSubMenu = GetSubMenu(ret->hMenu, 0)))
	{	delete ret;
		return 0;	}

	ret->hwnd = hwnd;
	ret->lParam = 0;

	return ret;
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