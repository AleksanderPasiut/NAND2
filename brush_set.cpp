#include <stdexcept>

#include "brush_set.h"

BRUSH_SET::BRUSH_SET(ID2D1HwndRenderTarget* target, IDWriteFactory* dwfactory)
{
	ID2D1Factory* factory;
	target->GetFactory(&factory);

	D2D1_COLOR_F color[BRUSH_AMOUNT];
	using namespace D2D1;
	color[0] = ColorF(ColorF::Azure);
	color[1] = ColorF(ColorF::Black);
	color[2] = ColorF(ColorF::DarkGray);
	color[3] = ColorF(ColorF::Gray);
	color[4] = ColorF(ColorF::LightGray);
	color[5] = ColorF(ColorF::DarkRed);
	color[6] = ColorF(ColorF::Red);
	color[7] = ColorF(ColorF::Yellow);
	color[8] = ColorF(ColorF::Green);
	color[9] = ColorF(ColorF::Orange);

	try
	{
		try
		{
			for (int i = 0; i < BRUSH_AMOUNT; i++)
				if (S_OK != target->CreateSolidColorBrush(color[i], &brush[i]))
					throw BRUSH_AMOUNT-i;
		}
		catch (int failed_brush_amount)
		{
			for (int i = 0; i < failed_brush_amount; i++)
				brush[i]->Release();

			throw 0;
		}

		if (S_OK != factory->CreateStrokeStyle(D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND), 0, 0, &stroke))
			throw 1;

		if (S_OK != dwfactory->CreateTextFormat(L"Verdana",
												0,
												DWRITE_FONT_WEIGHT_NORMAL,
												DWRITE_FONT_STYLE_NORMAL,
												DWRITE_FONT_STRETCH_NORMAL,
												20.0f,
												L"pl-utf8",
												&big_font))
			throw 2;

		big_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		big_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		if (S_OK != dwfactory->CreateTextFormat(L"Verdana",
												0,
												DWRITE_FONT_WEIGHT_NORMAL,
												DWRITE_FONT_STYLE_NORMAL,
												DWRITE_FONT_STRETCH_NORMAL,
												14.0f,
												L"pl-utf8",
												&normal_font))
			throw 3;

		if (S_OK != dwfactory->CreateTextFormat(L"Verdana",
												0,
												DWRITE_FONT_WEIGHT_NORMAL,
												DWRITE_FONT_STYLE_NORMAL,
												DWRITE_FONT_STRETCH_NORMAL,
												12.0f,
												L"pl-utf8",
												&small_font))
			throw 4;

		small_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		big_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	catch (int error_code)
	{
		switch(error_code)
		{
		case 4: normal_font->Release();
		case 3: big_font->Release();
		case 2: stroke->Release();
		case 1: for (int i = 0; i < BRUSH_AMOUNT; i++)
					brush[i]->Release();
		case 0: throw std::bad_alloc();
		}
	}
}

BRUSH_SET::~BRUSH_SET()
{
	for (int i = 0; i < BRUSH_AMOUNT; i++)
		brush[i]->Release();

	stroke->Release();
	big_font->Release();
	normal_font->Release();
	small_font->Release();
}