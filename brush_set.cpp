#include "brush_set.h"

BRUSH_SET* BRUSH_SET::Create(ID2D1HwndRenderTarget* target, IDWriteFactory* dwfactory)
{
	BRUSH_SET* ret = new BRUSH_SET;

	if (!ret)
		return 0;

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

	for (int i = 0; i < BRUSH_AMOUNT; i++)
		if (S_OK != target->CreateSolidColorBrush(color[i], &ret->brush[i]))
		{	delete ret;
			return 0;	}

	if (S_OK != factory->CreateStrokeStyle(D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND), 0, 0, &ret->stroke))
	{	delete ret;
		return 0;	}

	if (S_OK != dwfactory->CreateTextFormat(L"Verdana",
											0,
											DWRITE_FONT_WEIGHT_NORMAL,
											DWRITE_FONT_STYLE_NORMAL,
											DWRITE_FONT_STRETCH_NORMAL,
											20.0f,
											L"pl-utf8",
											&ret->big_font))
	{	delete ret;
		return 0;	}

	ret->big_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	ret->big_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (S_OK != dwfactory->CreateTextFormat(L"Verdana",
											0,
											DWRITE_FONT_WEIGHT_NORMAL,
											DWRITE_FONT_STYLE_NORMAL,
											DWRITE_FONT_STRETCH_NORMAL,
											14.0f,
											L"pl-utf8",
											&ret->normal_font))
	{	delete ret;
		return 0;	}

	if (S_OK != dwfactory->CreateTextFormat(L"Verdana",
											0,
											DWRITE_FONT_WEIGHT_NORMAL,
											DWRITE_FONT_STYLE_NORMAL,
											DWRITE_FONT_STRETCH_NORMAL,
											12.0f,
											L"pl-utf8",
											&ret->small_font))
	{	delete ret;
		return 0;	}

	ret->small_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	ret->big_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	return ret;
}

BRUSH_SET::~BRUSH_SET()
{
	for (int i = 0; i < BRUSH_AMOUNT; i++)
		if (brush[i])
			brush[i]->Release();

	if (stroke)		stroke->Release();
	if (big_font)	big_font->Release();
	if (normal_font)normal_font->Release();
	if (small_font) small_font->Release();
}