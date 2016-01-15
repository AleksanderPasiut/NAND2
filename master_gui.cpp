#include <WindowsX.h>
#include "master.h"
#include "dialog_resources.h"

void MASTER::MouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			unstable_system_notification = false;
			moving.click = sns.Click(lParam);

			ELEMENT* element = 0;
			EVPV evpv = EVPV_NONE;

			if (elements_set.RetAmount())
			{
				for (unsigned i = elements_set.RetAmount()-1; ; i--)
				{
					if ((evpv = elements_set[i]->MouseInput(moving.click)).type != EVPV_NONE)
					{	element = elements_set[i];
						break;	}

					if (!i)
						break;
				}
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
					SetCapture(hwnd);
					break;
				}
			case EVPV_BODY:	
				{
					moving.element = element;
					moving.old_pos = moving.element->RetPos();
					SetCapture(hwnd);
					break;
				}
			case EVPV_CONTROL:
				{
					if (element->RetSourceFlag())
						Proceed(element);
					break;
				}
			case EVPV_CROSS:
				{
					RemoveElement(element);
					Proceed(0);
					break;
				}
			case EVPV_INPUT:
			case EVPV_OUTPUT:
				{
					Link(element, evpv);
					Proceed(0);
					break;
				}
			}

			break;
		}
	case WM_MOUSEMOVE:
		{
			if (moving.element)
			{
				D2D1_POINT_2F new_pos = sns.Click(lParam);

				new_pos.x = moving.old_pos.x + (new_pos.x-moving.click.x);
				new_pos.y = moving.old_pos.y + (new_pos.y-moving.click.y);

				if (new_pos.x > sns.TRANSLATION_LIMIT)
					new_pos.x = sns.TRANSLATION_LIMIT;
				if (new_pos.x < -sns.TRANSLATION_LIMIT)
					new_pos.x = -sns.TRANSLATION_LIMIT;
				if (new_pos.y > sns.TRANSLATION_LIMIT)
					new_pos.y = sns.TRANSLATION_LIMIT;
				if (new_pos.y < -sns.TRANSLATION_LIMIT)
					new_pos.y = -sns.TRANSLATION_LIMIT;

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
			if (!ScreenToClient(hwnd, &pt))
				break;

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				if (sns.scale*sns.SCALE_MULTIPLIER < sns.SCALE_LIMIT_TOP)
					sns.SetScale(&pt, sns.scale*sns.SCALE_MULTIPLIER);
			}
			else 
			{
				if (sns.scale/sns.SCALE_MULTIPLIER > sns.SCALE_LIMIT_BOTTOM)
					sns.SetScale(&pt, sns.scale/sns.SCALE_MULTIPLIER);
			}

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

	if (linking.mark_all_inputs)
		PaintInputs();

	if (linking.mark_all_outputs)
		PaintOutputs();

	for (unsigned i = 0; i < elements_set.RetAmount(); i++)
		elements_set[i]->PaintWires();

	target->EndDraw();
	EndPaint(hwnd, 0);
	return;
}
void MASTER::Size(WPARAM wParam, LPARAM lParam)
{
	target->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));

	sns.RefreshMatrix();
	Paint();
	return;
}
void MASTER::ShowWindow()
{
	target->Resize(RetHwndClientSize(hwnd));
	sns.RefreshMatrix();
	Paint();
	return;
}
unsigned MASTER::RetNewElementId()
{
	unsigned ret;
	for (ret = 1; ret < ELEMENT_ID_LIMIT; ret++)
	{
		bool exists = false;

		for (unsigned i = 0; i < elements_set.RetAmount(); i++)
			if (elements_set[i]->RetId() == ret)
			{	exists = true;
				break;	}

		if (!exists)
			return ret;
	}
	return 0;
}
ELEMENT* MASTER::Nand(unsigned input_amount, const D2D1_POINT_2F& position)
{
	return ELEMENT_NAND::Create(target,
								brush_set,
								position.x,
								position.y,
								RetNewElementId(),
								input_amount);
}
void MASTER::MenuInput(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) || lParam)
		return;

	// dodawany element
	ELEMENT* element = 0;

	// jego po³o¿enie
	D2D1_POINT_2F position = sns.Click(menu->RetPos());

	switch(LOWORD(wParam))
	{
	case MENU_ADD_SOURCE:
		{
			element = ELEMENT_SOURCE::Create(target,
											 brush_set,
											 position.x,
											 position.y,
											 RetNewElementId());
			break;
		}
	case MENU_ADD_CLOCK:
		{
			if (unsigned elapse = static_cast<unsigned>(DialogBox(0, "res_dialog_add_clock", hwnd, reinterpret_cast<DLGPROC>(AddClockDialogProc))))
			{
				element = ELEMENT_CLOCK::Create(target,
												brush_set,
												position.x,
												position.y,
												RetNewElementId(),
												elapse,
												this);
			}
			break;
		}
	case MENU_ADD_NAND2: element = Nand(2, position); break;
	case MENU_ADD_NAND3: element = Nand(3, position); break;
	case MENU_ADD_NAND4: element = Nand(4, position); break;
	case MENU_ADD_NAND: 
		{
			if (unsigned i = static_cast<unsigned>(DialogBox(0, "res_dialog_add_nand", hwnd, reinterpret_cast<DLGPROC>(AddNandDialogProc))))
				element = Nand(i, position);
			break;
		}
	case MENU_ADD_OUTPUT:
		{
			element = ELEMENT_OUTPUT::Create(target,
											 brush_set,
											 position.x,
											 position.y,
											 RetNewElementId());
			break;
		}
	case MENU_ADD_COMMENT:
		{
			const COMMENT_DIALOG_RET_VALUE* cdrv = reinterpret_cast<COMMENT_DIALOG_RET_VALUE*>(DialogBox(0, "res_dialog_add_comment", hwnd, reinterpret_cast<DLGPROC>(AddCommentDialogProc)));
			if (cdrv->text)
			{
				element = ELEMENT_COMMENT::Create(target,
												  brush_set,
												  position.x,
												  position.y,
												  cdrv->text,
												  dwfactory);
			}
			break;
		}
	case MENU_CLEAR_BOARD:
		{
			if (IDYES == MessageBox(hwnd, "Czy na pewno chcesz usun¹æ wszystkie elementy?", "Usuwanie elementów", MB_YESNO))
				elements_set.clear();
			break;
		}
	}

	if (element)
		elements_set.add(element);

	Paint();
	return;
}