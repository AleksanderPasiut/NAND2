#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

#include "brush_set.h"
#include "elements_set.h"
#include "menu.h"
#include "saveload.h"

#include "element_source.h"
#include "element_nand.h"
#include "element_clock.h"
#include "element_jk.h"
#include "element_output.h"
#include "element_comment.h"

class MASTER
{
	NONCOPYABLE(MASTER)

private:
	HWND hwnd;
	ID2D1Factory* factory;
	IDWriteFactory* dwfactory;
	ID2D1HwndRenderTarget* target;
	BRUSH_SET* brush_set;
	ELEMENTS_SET elements_set;
	MENU* menu;
	SAVELOAD* saveload;

	bool unstable_system_notification;

	struct MOVING
	{
		ELEMENT* element;
		D2D1_POINT_2F old_pos;
		D2D1_POINT_2F click;

		MOVING() : element(0), old_pos(D2D1::Point2F()), click(D2D1::Point2F()) {}
	};
	MOVING moving;

	struct LINKING
	{
		EVPV_TYPE type;
		ELEMENT* element;
		unsigned id;

		bool mark_all_inputs;
		bool mark_all_outputs;

		LINKING() : type(EVPV_NONE), element(0), id(0), mark_all_inputs(false), mark_all_outputs(false) {}
	};
	LINKING linking;

	// master_scrolling_n_scaling.cpp
	struct SCROLLING_N_SCALING
	{
		static const float SCALE_MULTIPLIER;
		static const float SCALE_LIMIT_TOP;
		static const float SCALE_LIMIT_BOTTOM;
		static const float TRANSLATION_LIMIT;

		ID2D1HwndRenderTarget* target;
		float x;
		float y;
		float tx;
		float ty;
		float scale;
		D2D1_MATRIX_3X2_F transform; // macierz transformuj�ca z uk�adu logicznego (uk�ad element�w) do ekranu
		bool moving;

		SCROLLING_N_SCALING() : target(0), x(0.0f), y(0.0f), tx(0.0f), ty(0.0f), scale(1.0f), transform(D2D1::IdentityMatrix()), moving(false) {}

		void StartMoving();
		void SetTranslation(float x, float y);
		void SetScale(const POINT* pt, float scale);
		void RefreshMatrix();
		D2D1_POINT_2F Click(LPARAM lParam) const;
		D2D1_POINT_2F Click(const POINT* pt) const;
	};
	SCROLLING_N_SCALING sns;

	static const int ELEMENT_ID_LIMIT = 10000000;

private:
	// master_create.cpp
	static D2D1_SIZE_U RetHwndClientSize(HWND); 

private:
	// master_link.cpp
	void Link(ELEMENT*, EVPV);
	void PaintInputs();
	void PaintOutputs();
	void RemoveElement(ELEMENT*);

public:
	// master_create.cpp
	MASTER(HWND);
	~MASTER();

	// master_gui.cpp
	void MouseInput(UINT, WPARAM, LPARAM);
	void Paint();
	void Size(WPARAM, LPARAM);
	unsigned RetNewElementId();
	ELEMENT* Nand(unsigned, const D2D1_POINT_2F&);
	void MenuInput(WPARAM, LPARAM);
	void ShowWindow();

	// master_proceed.cpp
	void Proceed(ELEMENT* starting_element);

	void Save() { saveload->Save(); }
	HWND RetHwnd() { return hwnd; }

	friend class SAVELOAD;
};