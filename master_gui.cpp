#include <WindowsX.h>
#include "master.h"

void MASTER::MouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			if (element_moved)
			{
				element_moved = 0;
			}
			else
			{
				D2D1_POINT_2F click = D2D1::Point2F(static_cast<float>(GET_X_LPARAM(lParam)),
													static_cast<float>(GET_Y_LPARAM(lParam)));

				ELEMENT* element = 0;
				EVPV evpv;
				for (unsigned i = 0; i < elements_set.RetAmount(); i++)
					if ((evpv = elements_set[i]->MouseInput(click)).type != EVPV_NONE)
					{	element = elements_set[i];
						break;	}

				switch(evpv.type)
				{
				case EVPV_NONE: break;
				case EVPV_BODY:	element_moved = element; break;
				}
			}

			break;
		}
	case WM_MOUSEMOVE:
		{
			if (element_moved)
			{
				D2D1_POINT_2F old_pos = element_moved->RetPos();
			}

			break;
		}
	case WM_RBUTTONDOWN:
		{
			menu->Show(lParam);
			break;
		}
	}
	return;
}
void MASTER::Paint()
{
	BeginPaint(hwnd, 0);
	target->BeginDraw();
	
	target->Clear(D2D1::ColorF(D2D1::ColorF::Azure));

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		elements_set[i]->Paint();

	target->EndDraw();
	EndPaint(hwnd, 0);
	return;
}
void MASTER::Size(WPARAM wParam, LPARAM lParam)
{
	target->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));

	Paint();
	return;
}
void MASTER::MenuInput(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) || lParam)
		return;

	ELEMENT* new_element = new ELEMENT(target, brush_set, text_format, 0.1f, 0.1f, 0.2f, 0.2f);

	elements_set.add(new_element);
	Paint();
	/*
	switch(LOWORD(wParam))
	{
	case MENU_ADD_SOURCE: MessageBox(hwnd, "Dodanie Ÿród³a", "Tmp", MB_OK); break;
	case MENU_ADD_CLOCK: MessageBox(hwnd, "Dodanie zegara", "Tmp", MB_OK); break;
	case MENU_ADD_NAND2: MessageBox(hwnd, "Dodanie NAND2", "Tmp", MB_OK); break;
	case MENU_ADD_NAND3: MessageBox(hwnd, "Dodanie NAND3", "Tmp", MB_OK); break;
	case MENU_ADD_NAND4: MessageBox(hwnd, "Dodanie NAND4", "Tmp", MB_OK); break;
	case MENU_ADD_NAND: MessageBox(hwnd, "Dodanie NAND", "Tmp", MB_OK); break;
	}*/
	return;
}