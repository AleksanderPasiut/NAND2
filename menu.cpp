#include "menu.h"

MENU* MENU::Create(HWND hwnd)
{
	MENU* ret = new MENU;

	ret->hwnd = hwnd;

	if (ret)
	{
		if (ret->hMenu = LoadMenu(0, "res_popup_menu"))
		{
			if (ret->hSubMenu = GetSubMenu(ret->hMenu, 0))
			{
				return ret;
			}
			DestroyMenu(ret->hMenu);
		}
		delete ret;
	}

	return 0;
}
MENU::~MENU()
{
	if (ok)
	{
		DestroyMenu(hSubMenu);
		DestroyMenu(hMenu);
	}
}

void MENU::Show(LPARAM lParam)
{
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	if (!ClientToScreen(hwnd, &pt))
		return;
	TrackPopupMenu(hSubMenu, 0, pt.x, pt.y, 0, hwnd, 0);
	return;
}