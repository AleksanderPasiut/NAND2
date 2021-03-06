#pragma once

#define MENU_ADD_SOURCE 0x101
#define MENU_ADD_CLOCK 0x102
#define MENU_ADD_NAND2 0x103
#define MENU_ADD_NAND3 0x104
#define MENU_ADD_NAND4 0x105
#define MENU_ADD_NAND 0x106
#define MENU_ADD_OUTPUT 0x107
#define MENU_ADD_FLIP_FLOP 0x108
#define MENU_ADD_COMMENT 0x109
#define MENU_CLEAR_BOARD 0x10a

#include <Windows.h>

#include "noncopyable.h"

class MENU
{
	NONCOPYABLE(MENU)

private:
	HWND hwnd;
	HMENU hMenu;
	HMENU hSubMenu;
	LPARAM lParam;

public:
	MENU(HWND);
	~MENU();

	void Show(LPARAM);
	LPARAM RetPos() const { return lParam; }
};