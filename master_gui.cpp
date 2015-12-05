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

			moving.click = sns.Click(lParam);

			ELEMENT* element = 0;
			EVPV evpv = EVPV_NONE;

			for (unsigned i = elements_set.RetAmount()-1; ; i--)
			{
				if ((evpv = elements_set[i]->MouseInput(moving.click)).type != EVPV_NONE)
				{	element = elements_set[i];
					break;	}

				if (!i)
					break;
			}

			if (element)
				elements_set.set_on_top(element);

			switch(evpv.type)
			{
			case EVPV_NONE:
				{
					moving.click.x = static_cast<float>(GET_X_LPARAM(lParam));
					moving.click.y = static_cast<float>(GET_Y_LPARAM(lParam));
					sns.StartMoving();
					break;
				}
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

			Proceed();
			break;
		}
	case WM_MOUSEMOVE:
		{
			if (moving.element)
			{
				D2D1_SIZE_F ts = target->GetSize();
				D2D1_POINT_2F new_pos = sns.Click(lParam);

				new_pos.x = moving.old_pos.x + (new_pos.x-moving.click.x)/ts.width;
				new_pos.y = moving.old_pos.y + (new_pos.y-moving.click.y)/ts.height;

				moving.element->SetPos(new_pos);
				Paint();
			}
			else if (sns.moving)
			{
				D2D1_POINT_2F pt;
				pt.x = static_cast<float>(GET_X_LPARAM(lParam))-moving.click.x;
				pt.y = static_cast<float>(GET_Y_LPARAM(lParam))-moving.click.y;
				sns.SetTranslation(sns.tx+pt.x, sns.ty+pt.y);
				Paint();
			}

			break;
		}
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			moving.element = 0;
			sns.moving = false;
			break;
		}
	case WM_RBUTTONDOWN:
		{
			menu->Show(lParam);
			break;
		}
	case WM_MOUSEWHEEL:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			ScreenToClient(hwnd, &pt);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				sns.SetScale(&pt, sns.scale*1.5f);
			else sns.SetScale(&pt, sns.scale/1.5f);
			Paint();
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
	target->SetTransform(sns.transform);

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
								input_amount == 2 ? 0.11f : input_amount*0.04f,
								0,
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
											 0.1f,
											 0);
			break;
		}
	case MENU_ADD_CLOCK:
		{
			if (unsigned elapse = static_cast<unsigned>(DialogBox(0, "res_dialog_add_clock", hwnd, reinterpret_cast<DLGPROC>(AddClockDialogProc))))
			{
				element = ELEMENT_CLOCK::Create(target,
												brush_set,
												text_format,
												static_cast<float>(GET_X_LPARAM(menu->RetPos()))/ts.width,
												static_cast<float>(GET_Y_LPARAM(menu->RetPos()))/ts.height,
												0.08f,
												0.15f,
												0,
												elapse,
												this);
			}
			break;
		}
	case MENU_ADD_NAND2: element = Nand(2); break;
	case MENU_ADD_NAND3: element = Nand(3);	break;
	case MENU_ADD_NAND4: element = Nand(4);	break;
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
											 0.1f,
											 0);
			break;
		}
	}

	if (element)
		elements_set.add(element);

	Paint();
	return;
}