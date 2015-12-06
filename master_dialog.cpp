#include "dialog_resources.h"
#include <stdlib.h>

BOOL CALLBACK AddClockDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ACTIVATE:
		{
			SetFocus(GetDlgItem(hwnd, CTRL_EDIT));
			return FALSE;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK: 
				{
					char buffer[6];
					GetDlgItemText(hwnd, CTRL_EDIT, buffer, 6);
					unsigned value = static_cast<unsigned>(atoi(buffer));
					EndDialog(hwnd, 50 <= value && value <= 60000 ? value : 0);
					return TRUE;
				}
			case IDCANCEL:
				{
					EndDialog(hwnd, 0);
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

BOOL CALLBACK AddNandDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ACTIVATE:
		{
			SetFocus(GetDlgItem(hwnd, CTRL_EDIT));
			return FALSE;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK: 
				{
					char buffer[4];
					GetDlgItemText(hwnd, CTRL_EDIT, buffer, 4);
					unsigned value = static_cast<unsigned>(atoi(buffer));
					EndDialog(hwnd, 2 <= value && value <= 10 ? value : 0);
					return TRUE;
				}
			case IDCANCEL:
				{
					EndDialog(hwnd, 0);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CALLBACK AddCommentDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ACTIVATE:
		{
			SetFocus(GetDlgItem(hwnd, CTRL_EDIT));
			return FALSE;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK: 
				{
					static COMMENT_DIALOG_RET_VALUE cdrv;
					int size = GetWindowTextLength(GetDlgItem(hwnd, CTRL_EDIT))+1;
					cdrv.CreateText(size);
					GetDlgItemText(hwnd, CTRL_EDIT, cdrv.text, size);
					EndDialog(hwnd, reinterpret_cast<INT_PTR>(&cdrv));
					return TRUE;
				}
			case IDCANCEL:
				{
					EndDialog(hwnd, 0);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}