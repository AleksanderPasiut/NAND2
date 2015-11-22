#include "dialog_resources.h"

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
