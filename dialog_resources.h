#pragma once

#define CTRL_NUMB 100
#define CTRL_EDIT 101
#define CTRL_CHECKBOX 102

#include <Windows.h>

BOOL CALLBACK AddClockDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AddNandDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AddTextboxDialogProc(HWND, UINT, WPARAM, LPARAM);

struct ADD_TEXTBOX_RV
{
	char text[256];
	bool pointer;
};