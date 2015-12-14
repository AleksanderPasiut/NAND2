#include "master.h"

D2D1_SIZE_U MASTER::RetHwndClientSize(HWND hwnd)
{
	RECT rect;
	if (GetClientRect(hwnd, &rect))
		return D2D1::SizeU(rect.right-rect.left, rect.bottom-rect.top);
	else return D2D1::SizeU();
}

MASTER* MASTER::Create(HWND hwnd)
{
	MASTER* ret = new MASTER;

	if (!ret)
		return 0;

	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &ret->factory))
	{	delete ret;
		return 0;	}

	if (S_OK != DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&ret->dwfactory)))
	{	delete ret;
		return 0;	}

	if (S_OK != ret->factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
													 D2D1::HwndRenderTargetProperties(hwnd, RetHwndClientSize(hwnd)), 
													 &ret->target))
	{	delete ret;
		return 0;	}

	if (!(ret->brush_set = BRUSH_SET::Create(ret->target, ret->dwfactory)))
	{	delete ret;
		return 0;	}

	if (!(ret->menu = MENU::Create(hwnd)))
	{	delete ret;
		return 0;	}

	if (!(ret->saveload = SAVELOAD::Create(ret)))
	{	delete ret;
		return 0;	}

	ret->hwnd = hwnd;
	ret->sns.target = ret->target;
	ret->sns.RefreshMatrix();
	if (!ret->saveload->Load())
		::ShowWindow(hwnd, SW_SHOWNORMAL);
	return ret;
}

MASTER::~MASTER()
{
	delete saveload;
	delete menu;
	delete brush_set;
	if (target)			target->Release();
	if (dwfactory)		dwfactory->Release();
	if (factory)		factory->Release();
}

