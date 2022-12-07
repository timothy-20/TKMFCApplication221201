#pragma once
#include "TKBaseWindow.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

template<typename T>
void TKSafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class TKMainWindow : public TKBaseWindow<TKMainWindow>
{
private:
	ID2D1Factory* p_factory;
	ID2D1HwndRenderTarget* p_renderTarget;
	ID2D1SolidColorBrush* p_brush;
	D2D1_ELLIPSE ellipse;

	void CalculateLayout();
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnPaint();
	void ReSize();

public:
	TKMainWindow() : p_factory(NULL), p_renderTarget(NULL), p_brush(NULL) { }
	PCWSTR ClassName() const { return L"TKMainWindow"; }
	LRESULT HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam);
};