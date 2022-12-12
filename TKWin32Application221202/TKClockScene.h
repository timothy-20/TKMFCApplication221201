#pragma once
#include "TKGraphicScene.h"

class TKClockScene : public TKGraphicScene
{
private:
	CComPtr<ID2D1SolidColorBrush> m_Fill;
	CComPtr<ID2D1SolidColorBrush> m_Stroke;
	D2D1_ELLIPSE m_ellipse;
	D2D_POINT_2F m_Ticks[24];

	HRESULT CreateDeviceIndependentResources() { return S_OK; }
	void DiscardDeviceIndependentResources() {}
	HRESULT CreateDeviceDependentResources();
	void DiscardDeviceDependentResources();
	void CalculateLayout();
	void RenderScene();
	void DrawClockHand(float handLength, float angle, float strokeWidth);
};

