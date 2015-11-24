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
						if (ret->menu = MENU::Create(hwnd))
						{
							ret->hwnd = hwnd;
							return ret;

							delete ret->menu;
							ret->menu = 0;
						}
						delete ret->brush_set;
						ret->brush_set = 0;
					}
					ret->target->Release();
					ret->target = 0;
				}
				ret->dwfactory->Release();
				ret->dwfactory = 0;
			}	
			ret->factory->Release();
			ret->factory = 0;
		}
		delete ret;
	}
	return 0;
}

MASTER::~MASTER()
{
	delete menu;
	delete brush_set;
	if (target)			target->Release();
	if (dwfactory)		dwfactory->Release();
	if (factory)		factory->Release();
}

