#include <Windows.h>

#include "master.h"
MASTER* Master = 0;

char ClassName[] = "NAND2AppClass";

LRESULT CALLBACK WindowEventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_CAPTURECHANGED:
	case WM_RBUTTONDOWN:
		{
			if (Master)
				Master->MouseInput(uMsg, wParam, lParam);

			break;
		}
	case WM_PAINT:
		{
			if (Master)
				Master->Paint();

			break;
		}
	case WM_SIZE:
		{
			if (Master)
				Master->Size(wParam, lParam);

			break;
		}
	case WM_COMMAND:
		{
			if (Master)
				Master->MenuInput(wParam, lParam);

			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	int ret_value = 0;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowEventProc;
	wc.lpszClassName = ClassName;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	
	if (RegisterClassEx(&wc))
	{
		HWND hwnd = CreateWindowEx(0,
								   ClassName,
								   "NAND2",
								   WS_OVERLAPPEDWINDOW,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   0,
								   0,
								   hInstance,
								   0);

		if (hwnd)
		{
			if (Master = MASTER::Create(hwnd))
			{
				ShowWindow(hwnd, nCmdShow);

				MSG msg;
				while(GetMessage(&msg, 0, 0, 0))
				{	TranslateMessage(&msg);
					DispatchMessage(&msg);	}

				ret_value = static_cast<int>(msg.wParam);

				delete Master;
			}

			DestroyWindow(hwnd);
		}

		UnregisterClass(ClassName, hInstance);
	}
	return ret_value;
}