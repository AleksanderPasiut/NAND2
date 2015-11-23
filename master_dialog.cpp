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
					char buffer[4];
					GetDlgItemText(hwnd, CTRL_EDIT, buffer, 4);
					unsigned value = static_cast<unsigned>(atoi(buffer));
					EndDialog(hwnd, value);
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

BOOL CALLBACK AddTextboxDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
					EndDialog(hwnd, wParam);
					return TRUE;
				}
			case IDCANCEL:
				{
					EndDialog(hwnd, wParam);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
