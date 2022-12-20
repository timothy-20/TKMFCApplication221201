#pragma once
#include "TKWindow.h"

class TKTextField
{
private:
	HWND m_hWndBackground;
	HWND m_hWndTitleLabel;
	HWND m_hWndTextField;
	TKWindowFrame m_frame;
	LPCWSTR m_title;
	LPCWSTR m_placeholder;

public:
	TKTextField(const TKWindowFrame& frame);

	// Utils
	void CreateTextField(HWND hWndParent);
	void SetTitleText(const LPCWSTR& text) const;
	void SetFont(const LPCWSTR& fontName, uint32_t fontSize) const;
	void SetRect(const TKWindowFrame& frame) const;
	TKWindowFrame GetRect() const;
};