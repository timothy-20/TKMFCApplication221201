#pragma once
#include <Windows.h>

class TKDPIScale
{
private:
	float m_x;
	float m_y;

public:
	TKDPIScale(HWND hwnd)
	{
		HDC hdc = GetDC(hwnd);
		this->m_x = ::GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
		this->m_y = ::GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
		::ReleaseDC(hwnd, hdc);
	}

	TKDPIScale(HWND hwnd, float x, float y)
	{
		const float dpi = ::GetDpiForWindow(hwnd);
		const float dpiScale = dpi / 96.0F;
		this->m_x = static_cast<float>(x) / dpiScale;
		this->m_y = static_cast<float>(y) / dpiScale;
	}

	const decltype(m_x)& x() const { return this->m_x; }
	const decltype(m_y)& y() const { return this->m_y; }
};