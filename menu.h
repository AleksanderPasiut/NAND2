#pragma once

#define MENU_ADD_SOURCE 0x101
#define MENU_ADD_CLOCK 0x102
#define MENU_ADD_NAND2 0x103
#define MENU_ADD_NAND3 0x104
#define MENU_ADD_NAND4 0x105
#define MENU_ADD_NAND 0x106
#define MENU_ADD_OUTPUT 0x107

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