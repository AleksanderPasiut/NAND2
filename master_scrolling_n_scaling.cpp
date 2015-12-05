#include <WindowsX.h>
#include "master.h"

const float MASTER::SCROLLING_N_SCALING::SCALE_MULTIPLIER = 1.2f;
const float MASTER::SCROLLING_N_SCALING::SCALE_LIMIT_TOP = 12.0f;
const float MASTER::SCROLLING_N_SCALING::SCALE_LIMIT_BOTTOM = 0.1f;
const float MASTER::SCROLLING_N_SCALING::TRANSLATION_LIMIT = 1677721.0f; // float zachowuje w tym zakresie dok³adnoœæ do 0.1f

void MASTER::SCROLLING_N_SCALING::StartMoving()
{
	tx = x;
	ty = y;
	moving = true;
	return;
}
void MASTER::SCROLLING_N_SCALING::SetTranslation(float x, float y)
{
	this->x = x;
	this->y = y;
	RefreshMatrix();
	return;
}
void MASTER::SCROLLING_N_SCALING::SetScale(const POINT* pt, float scale)
{
	// zmiana przesuniêcia, aby skalowanie odbywa³o siê wokó³ punktu przekazanego przez pt
	D2D1_POINT_2F center = Click(pt);
	x += center.x*(this->scale-scale);
	y += center.y*(this->scale-scale);

	// ustawienie nowej skali
	this->scale = scale;

	// odœwie¿enie macierzy
	RefreshMatrix();
	return;
}
void MASTER::SCROLLING_N_SCALING::RefreshMatrix()
{
	// uwzglêdnienie limitów
	if (x > TRANSLATION_LIMIT*scale)
		x = TRANSLATION_LIMIT*scale;
	if (y > TRANSLATION_LIMIT*scale)
		y = TRANSLATION_LIMIT*scale;
	if (x < -TRANSLATION_LIMIT*scale)
		x = -TRANSLATION_LIMIT*scale;
	if (y < -TRANSLATION_LIMIT*scale)
		y = -TRANSLATION_LIMIT*scale;

	// odœwie¿enie macierzy
	D2D1_SIZE_F ts = target->GetSize();
	transform = D2D1::Matrix3x2F::Scale(scale, scale)*D2D1::Matrix3x2F::Translation(ts.width/2+x, ts.height/2+y);
	return;
}

D2D1_POINT_2F MASTER::SCROLLING_N_SCALING::Click(LPARAM lParam) const
{
	float x = static_cast<float>(GET_X_LPARAM(lParam));
	float y = static_cast<float>(GET_Y_LPARAM(lParam));

	float det = transform._11*transform._22-transform._12*transform._21;
	D2D1_POINT_2F ret;
	ret.x = (transform._22*(x-transform._31)-transform._12*(y-transform._32))/det;
	ret.y = (transform._11*(y-transform._32)-transform._21*(x-transform._31))/det;
	return ret;
}
D2D1_POINT_2F MASTER::SCROLLING_N_SCALING::Click(const POINT* pt) const
{
	float x = static_cast<float>(pt->x);
	float y = static_cast<float>(pt->y);

	float det = transform._11*transform._22-transform._12*transform._21;
	D2D1_POINT_2F ret;
	ret.x = (transform._22*(x-transform._31)-transform._12*(y-transform._32))/det;
	ret.y = (transform._11*(y-transform._32)-transform._21*(x-transform._31))/det;
	return ret;
}