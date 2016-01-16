#include "element_comment.h"

const float ELEMENT_COMMENT::TEXT_MARGIN_TOP = 2*CROSS_MARGIN+CROSS_SIZE;
const float ELEMENT_COMMENT::TEXT_MARGIN = 5.0f;

ELEMENT_COMMENT::ELEMENT_COMMENT(ID2D1HwndRenderTarget* target,
								 BRUSH_SET* brush_set,
								 float pos_x,
								 float pos_y,
								 float width,
								 float height)
	: ELEMENT(target, brush_set, pos_x, pos_y, width, height, 0, false) {}

ELEMENT_COMMENT::~ELEMENT_COMMENT()
{
	text_layout->Release();
	delete[] text;
}

ELEMENT_COMMENT* ELEMENT_COMMENT::Create(ID2D1HwndRenderTarget* target,
										 BRUSH_SET* brush_set,
										 float pos_x,
										 float pos_y,
										 const char* text,
										 IDWriteFactory* dwfactory)
{
	unsigned text_length = static_cast<unsigned>(strlen(text));
	wchar_t* wtext = new wchar_t [text_length+1];
	wtext[text_length] = 0;
	if (!wtext)
		return 0;
	MultiByteToWideChar(CP_ACP, 0, text, text_length, wtext, text_length+1);

	IDWriteTextLayout* layout;
	static const float max_width = 250.0f;
	static const float max_height = 5000.0f;
	if (S_OK != dwfactory->CreateTextLayout(wtext,
											text_length,
											brush_set->Font(),
											max_width,
											max_height,
											&layout))
		return 0;

	DWRITE_TEXT_METRICS metrics;
	layout->GetMetrics(&metrics);

	ELEMENT_COMMENT* ret = 0;
	try
	{
		ret = new ELEMENT_COMMENT(target,
									brush_set,
									pos_x,
									pos_y,
									metrics.width+2*TEXT_MARGIN,
									metrics.height+TEXT_MARGIN_TOP+TEXT_MARGIN);
	}
	catch(...)
	{
		layout->Release();
		return 0;
	}

	ret->text = wtext;
	ret->text_layout = layout;
	return ret;
}

ELEMENT_COMMENT* ELEMENT_COMMENT::Create(ID2D1HwndRenderTarget* target,
										 BRUSH_SET* brush_set,
										 float pos_x,
										 float pos_y,
										 const wchar_t* text,
										 IDWriteFactory* dwfactory)
{
	unsigned text_length = static_cast<unsigned>(wcslen(text));
	wchar_t* wtext = new wchar_t [text_length+1];
	wtext[text_length] = 0;
	if (!wtext)
		return 0;
	memcpy(wtext, text, (text_length+1)*sizeof(wchar_t));

	IDWriteTextLayout* layout;
	static const float max_width = 250.0f;
	static const float max_height = 5000.0f;
	if (S_OK != dwfactory->CreateTextLayout(wtext,
											text_length,
											brush_set->Font(),
											max_width,
											max_height,
											&layout))
		return 0;

	DWRITE_TEXT_METRICS metrics;
	layout->GetMetrics(&metrics);

	ELEMENT_COMMENT* ret = 0;
	try
	{
		ret = new ELEMENT_COMMENT(target,
									brush_set,
									pos_x,
									pos_y,
									metrics.width+2*TEXT_MARGIN,
									metrics.height+TEXT_MARGIN_TOP+TEXT_MARGIN);
	}
	catch(...)
	{	
		layout->Release();
		return 0;
	}

	ret->text = wtext;
	ret->text_layout = layout;
	return ret;
}


EVPV ELEMENT_COMMENT::MouseInput(const D2D1_POINT_2F& click)
{
	return ELEMENT::MouseInput(click);
}

void ELEMENT_COMMENT::Paint() const
{
	D2D1_RECT_F rect;
	RetElementRect(rect);
	target->DrawRectangle(rect, brush->Gray());

	RetCrossRect(rect);
	target->DrawRectangle(rect, brush->Gray());
	target->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.right, rect.bottom), brush->Gray());
	target->DrawLine(D2D1::Point2F(rect.left, rect.bottom), D2D1::Point2F(rect.right, rect.top), brush->Gray());
	
	target->DrawTextLayout(D2D1::Point2F(pos.x+TEXT_MARGIN, pos.y+TEXT_MARGIN_TOP), text_layout, brush->Black());
	return;
}