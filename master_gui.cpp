#include <WindowsX.h>
#include "master.h"

void MASTER::MouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			click = D2D1::Point2F(static_cast<float>(GET_X_LPARAM(lParam)),
									static_cast<float>(GET_Y_LPARAM(lParam)));

			ELEMENT* element = 0;
			EVPV evpv;
			for (unsigned i = elements_set.RetAmount()-1; ; i--)
			{
				if ((evpv = elements_set[i]->MouseInput(click)).type != EVPV_NONE)
				{	element = elements_set[i];
					break;	}

				if (!i)
					return;
			}

			elements_set.set_on_top(element);

			switch(evpv.type)
			{
			case EVPV_NONE: break;
			case EVPV_BODY:	
				{
					element_moved = element;
					old_pos = element_moved->RetPos();
					SetCapture(hwnd);
					break;
				}
			case EVPV_CROSS:
				{
					elements_set.remove(element);
					break;
				}
			}

			Paint();
			break;
		}
	case WM_MOUSEMOVE:
		{
			if (element_moved)
			{
				D2D1_SIZE_F ts = target->GetSize();
				D2D1_POINT_2F new_pos;

				new_pos.x = old_pos.x + (static_cast<float>(GET_X_LPARAM(lParam))-click.x)/ts.width;
				new_pos.y = old_pos.y + (static_cast<float>(GET_Y_LPARAM(lParam))-click.y)/ts.height;

				element_moved->SetPos(new_pos);
				
				Paint();
			}

			break;
		}
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			break;
		}
	case WM_CAPTURECHANGED:
		{
			element_moved = 0;
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

	D2D1_SIZE_F ts = target->GetSize();

	ELEMENT* new_element = new ELEMENT(target,
									   brush_set,
									   text_format,
									   static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
									   static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
									   0.2f,
									   0.2f);

	elements_set.add(new_element);
	Paint();
	/*
	switch(LOWORD(wParam))
	{
	case MENU_ADD_SOURCE: MessageBox(hwnd, "Dodanie �r�d�a", "Tmp", MB_OK); break;
	case MENU_ADD_CLOCK: MessageBox(hwnd, "Dodanie zegara", "Tmp", MB_OK); break;
	case MENU_ADD_NAND2: MessageBox(hwnd, "Dodanie NAND2", "Tmp", MB_OK); break;
	case MENU_ADD_NAND3: MessageBox(hwnd, "Dodanie NAND3", "Tmp", MB_OK); break;
	case MENU_ADD_NAND4: MessageBox(hwnd, "Dodanie NAND4", "Tmp", MB_OK); break;
	case MENU_ADD_NAND: MessageBox(hwnd, "Dodanie NAND", "Tmp", MB_OK); break;
	}*/
	return;
}