#pragma once

#define CTRL_NUMB 100
#define CTRL_EDIT 101

#include <Windows.h>

struct COMMENT_DIALOG_RET_VALUE
{
	char* text;

	COMMENT_DIALOG_RET_VALUE() : text(0) {}
	~COMMENT_DIALOG_RET_VALUE() { delete text; }

	void CreateText(int size) { delete[] text; text = new char[size]; return; }
};

BOOL CALLBACK AddClockDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AddNandDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AddCommentDialogProc(HWND, UINT, WPARAM, LPARAM);

