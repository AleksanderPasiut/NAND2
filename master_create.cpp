#include "master.h"

D2D1_SIZE_U MASTER::RetHwndClientSize(HWND hwnd)
{
	RECT rect;
	if (GetClientRect(hwnd, &rect))
		return D2D1::SizeU(rect.right-rect.left, rect.bottom-rect.top);
	else return D2D1::SizeU();
}

MASTER::MASTER(HWND in_hwnd) :
	hwnd(in_hwnd), 
	unstable_system_notification(false)
{
	try
	{
		if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory))
			throw 0;

		if (S_OK != DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwfactory)))
			throw 1;

		if (S_OK != factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
													D2D1::HwndRenderTargetProperties(hwnd, RetHwndClientSize(hwnd)),
													&target))
			throw 2;

		try { brush_set = new BRUSH_SET(target, dwfactory); }
		catch (...)	{ throw 3; }

		try { menu = new MENU(hwnd); }
		catch (...) { throw 4; }

		try { saveload = new SAVELOAD(this); }
		catch (...) { throw 5; }

		sns.target = target;
		sns.RefreshMatrix();
		if (!saveload->Load())
			::ShowWindow(hwnd, SW_SHOWNORMAL);
		else ShowWindow();
	}
	catch (int error_code)
	{
		switch(error_code)
		{
		case 5: delete menu;
		case 4: delete brush_set;
		case 3: target->Release();
		case 2: dwfactory->Release();
		case 1: factory->Release();
		case 0: throw std::bad_alloc();
		}
	}
}

MASTER::~MASTER()
{
	delete saveload;
	delete menu;
	delete brush_set;
	target->Release();
	dwfactory->Release();
	factory->Release();
}

