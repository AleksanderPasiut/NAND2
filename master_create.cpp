#include "master.h"

D2D1_SIZE_U MASTER::RetHwndClientSize(HWND hwnd)
{
	RECT rect;
	if (GetClientRect(hwnd, &rect))
		return D2D1::SizeU(rect.right-rect.left, rect.bottom-rect.top);
	else return D2D1::SizeU(0,0);
}

MASTER* MASTER::Create(HWND hwnd)
{
	MASTER* ret = new MASTER;

	if (ret)
	{
		if (S_OK == D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &ret->factory))
		{
			if (S_OK == DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&ret->dwfactory)))
			{
				if (S_OK == ret->factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
																 D2D1::HwndRenderTargetProperties(hwnd, RetHwndClientSize(hwnd)), 
																 &ret->target))
				{
					if (ret->brush_set = BRUSH_SET::Create(ret->target))
					{
						if (S_OK == ret->dwfactory->CreateTextFormat(L"Verdana",
																	 0,
																	 DWRITE_FONT_WEIGHT_NORMAL,
																	 DWRITE_FONT_STYLE_NORMAL,
																	 DWRITE_FONT_STRETCH_NORMAL,
																	 11.0f,
																	 L"pl-utf8",
																	 &ret->text_format))
						{
							if (ret->menu = MENU::Create(hwnd))
							{
								ret->ok = true;
								ret->hwnd = hwnd;
								ret->element_moved = 0;
								return ret;

								delete ret->menu;
							}
							ret->text_format->Release();
						}
						delete ret->brush_set;
					}
					ret->target->Release();
				}
				ret->dwfactory->Release();
			}	
			ret->factory->Release();
		}
		delete ret;
	}
	return 0;
}

MASTER::~MASTER()
{
	if (ok)
	{
		delete menu;
		text_format->Release();
		delete brush_set;
		target->Release();
		dwfactory->Release();
		factory->Release();
	}
}

