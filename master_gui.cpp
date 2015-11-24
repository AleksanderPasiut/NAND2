#include <WindowsX.h>
#include "master.h"
#include "dialog_resources.h"

void MASTER::MouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			if (!elements_set.RetAmount())
				break;

			moving.click = D2D1::Point2F(static_cast<float>(GET_X_LPARAM(lParam)),
										 static_cast<float>(GET_Y_LPARAM(lParam)));

			ELEMENT* element = 0;
			EVPV evpv;

			for (unsigned i = elements_set.RetAmount()-1; ; i--)
			{
				if ((evpv = elements_set[i]->MouseInput(moving.click)).type != EVPV_NONE)
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
					moving.element = element;
					moving.old_pos = moving.element->RetPos();
					SetCapture(hwnd);
					break;
				}
			case EVPV_CROSS:
				{
					RemoveElement(element);
					break;
				}
			case EVPV_INPUT:
			case EVPV_OUTPUT:
				{
					Link(element, evpv);
					break;
				}
			}

			Paint();
			break;
		}
	case WM_MOUSEMOVE:
		{
			if (moving.element)
			{
				D2D1_SIZE_F ts = target->GetSize();
				D2D1_POINT_2F new_pos;

				new_pos.x = moving.old_pos.x + (static_cast<float>(GET_X_LPARAM(lParam))-moving.click.x)/ts.width;
				new_pos.y = moving.old_pos.y + (static_cast<float>(GET_Y_LPARAM(lParam))-moving.click.y)/ts.height;

				moving.element->SetPos(new_pos);
				
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
			moving.element = 0;
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

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		elements_set[i]->PaintWires();

	if (linking.mark_all_inputs)
		PaintInputs();

	if (linking.mark_all_outputs)
		PaintOutputs();

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
ELEMENT* MASTER::Nand(unsigned input_amount)
{
	D2D1_SIZE_F ts = target->GetSize();
	return ELEMENT_NAND::Create(target,
								brush_set,
								text_format,
								static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
								static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
								0.11f,
								input_amount*0.04f,
								input_amount);
}
void MASTER::MenuInput(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) || lParam)
		return;

	D2D1_SIZE_F ts = target->GetSize();

	ELEMENT* element = 0;

	switch(LOWORD(wParam))
	{
	case MENU_ADD_SOURCE:
		{
			element = ELEMENT_SOURCE::Create(target,
											 brush_set,
											 text_format,
											 static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
											 static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
											 0.08f,
											 0.1f);
			break;
		}
	case MENU_ADD_CLOCK:
		{
			if (unsigned u = static_cast<unsigned>(DialogBox(0, "res_dialog_add_clock", hwnd, reinterpret_cast<DLGPROC>(AddClockDialogProc))))
			{
				element = ELEMENT_CLOCK::Create(target,
												brush_set,
												text_format,
												static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
												static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
												0.08f,
												0.15f,
												u,
												this);
			}
			break;
		}
	case MENU_ADD_NAND2:
		{
			element = Nand(2);
			break;
		}
	case MENU_ADD_NAND3:
		{
			element = Nand(3);
			break;
		}
	case MENU_ADD_NAND4:
		{
			element = Nand(4);
			break;
		}
	case MENU_ADD_NAND: 
		{
			if (unsigned i = static_cast<unsigned>(DialogBox(0, "res_dialog_add_nand", hwnd, reinterpret_cast<DLGPROC>(AddNandDialogProc))))
				element = Nand(i);
			break;
		}
	case MENU_ADD_OUTPUT:
		{
			element = ELEMENT_OUTPUT::Create(target,
											 brush_set,
											 text_format,
											 static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
											 static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
											 0.08f,
											 0.1f);
			break;
		}
	case MENU_ADD_TEXTBOX:
		{
			if (ADD_TEXTBOX_RV* rv = reinterpret_cast<ADD_TEXTBOX_RV*>(DialogBox(0, "res_dialog_add_textbox", hwnd, reinterpret_cast<DLGPROC>(AddTextboxDialogProc))))
			{
				if (rv->text[0])
				{
					element = ELEMENT_TEXTBOX::Create(target,
													  brush_set,
													  text_format,
													  static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
													  static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
													  0.08f,
													  0.1f,
													  rv->pointer,
													  rv->text);
				}
			}
			break;
		}
	}

	if (element)
		elements_set.add(element);

	Paint();
	return;
}