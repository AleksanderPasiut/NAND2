#include "brush_set.h"

BRUSH_SET* BRUSH_SET::Create(ID2D1HwndRenderTarget* target)
{
	BRUSH_SET* ret = new BRUSH_SET;

	if (ret)
	{
		ID2D1Factory* factory;
		target->GetFactory(&factory);

		if (S_OK == factory->CreateStrokeStyle(D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND), 0, 0, &ret->stroke))
		{
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
					for (int j = 0; j < i; j++)
					{
						ret->brush[j]->Release();
						ret->brush[0] = 0;

						delete ret;
						return 0;
					}

			return ret;
		}
		delete ret;
	}

	return 0;
}

BRUSH_SET::~BRUSH_SET()
{
	for (int i = 0; i < BRUSH_AMOUNT; i++)
		if (brush[i])
			brush[i]->Release();

	if (stroke)
		stroke->Release();
}