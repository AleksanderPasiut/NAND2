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
			if (S_OK == ret->factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
															 D2D1::HwndRenderTargetProperties(hwnd, RetHwndClientSize(hwnd)), 
															 &ret->target))
			{
				if (ret->brush_set = BRUSH_SET::Create(ret->target))
				{
					if (ret->elements_set = new ELEMENTS_SET())
					{
						if (ret->menu = MENU::Create(hwnd))
						{
							ret->ok = true;
							ret->hwnd = hwnd;
							return ret;

							delete ret->menu;
						}
						delete ret->elements_set;
					}
					delete ret->brush_set;
				}
				ret->target->Release();
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
		target->Release();
		factory->Release();
	}
}

