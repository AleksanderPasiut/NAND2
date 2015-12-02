#pragma once

#define CTRL_NUMB 100
#define CTRL_EDIT 101

#include <Windows.h>

BOOL CALLBACK AddClockDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AddNandDialogProc(HWND, UINT, WPARAM, LPARAM);
