#pragma once

#define MENU_ADD_SOURCE 0x101
#define MENU_ADD_CLOCK 0x102
#define MENU_ADD_NAND 0x103

#include <Windows.h>

class MENU
{
private:
	bool ok;

private:
	HWND hwnd;
	HMENU hMenu;
	HMENU hSubMenu;

private:
	MENU() : ok(false) {}
	MENU(const MENU&) {}
	MENU* operator= (const MENU&) {}

public:
	static MENU* Create(HWND);
	~MENU();

	void Show(LPARAM);
};