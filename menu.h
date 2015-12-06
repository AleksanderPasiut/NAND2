#pragma once

#define MENU_ADD_SOURCE 0x101
#define MENU_ADD_CLOCK 0x102
#define MENU_ADD_NAND2 0x103
#define MENU_ADD_NAND3 0x104
#define MENU_ADD_NAND4 0x105
#define MENU_ADD_NAND 0x106
#define MENU_ADD_OUTPUT 0x107
#define MENU_ADD_FLIP_FLOP 0x108

#include <Windows.h>

class MENU
{
private:
	HWND hwnd;
	HMENU hMenu;
	HMENU hSubMenu;
	LPARAM lParam;

private:
	MENU() { memset(this, 0, sizeof(MENU)); }
	MENU(const MENU&) {}
	MENU* operator= (const MENU&) {}

public:
	static MENU* Create(HWND);
	~MENU();

	void Show(LPARAM);
	LPARAM RetPos() const { return lParam; }
};