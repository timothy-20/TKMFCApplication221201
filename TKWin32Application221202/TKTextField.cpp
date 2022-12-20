#include "TKTextField.h"

struct WindowHandlers
{
private:
	HWND m_hWndBackground;
	HWND m_hWndTitleLabel;
	HWND m_hWndInputField;

public:
	WindowHandlers(HWND hWndBackground, HWND hWndTitleLabel, HWND hWndInputField) :
		m_hWndBackground(hWndBackground),
		m_hWndTitleLabel(hWndTitleLabel),
		m_hWndInputField(hWndInputField)
	{
	}

	void DefaultFontWithSize(unsigned int size)
	{
		HFONT hTitleFont(::CreateFont(
			size, 0, 0, 0, FW_MEDIUM,
			FALSE, FALSE, FALSE,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Arial"
		));

		::SendMessage(this->m_hWndTitleLabel, WM_SETFONT, WPARAM(hTitleFont), TRUE);
	}
};

TKTextField::TKTextField(const TKWindowFrame& frame) :
	m_hWndBackground(0),
	m_hWndTitleLabel(0),
	m_hWndTextField(0),
	m_frame(frame),
	m_title(L""),
	m_placeholder(L"")
{
}
void TKTextField::CreateTextField(HWND hWndParent)
{
	TKWindow background(TKWindow::PreviousWindowClass::STATIC, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER, this->m_frame);
	TKWindow titleLabel(TKWindow::PreviousWindowClass::STATIC, L"", WS_CHILD | WS_VISIBLE | SS_LEFT);
	TKWindow textField(TKWindow::PreviousWindowClass::EDIT, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL);
	this->m_hWndBackground = background.InsertToParentWindow(hWndParent);
	this->m_hWndTitleLabel = titleLabel.InsertToParentWindow(this->m_hWndBackground);
	this->m_hWndTextField = textField.InsertToParentWindow(this->m_hWndBackground);

	::SetWindowPos(this->m_hWndTitleLabel, nullptr, 10, ((this->m_frame.height - 20) * 0.5), 100, 20, SWP_DRAWFRAME);
	::SetWindowPos(this->m_hWndTextField, nullptr, 110, 10, (this->m_frame.width - 120), (this->m_frame.height - 20), SWP_DRAWFRAME);
	this->SetFont(L"Arial", 20);
}
void TKTextField::SetTitleText(const LPCWSTR& text) const
{
	::SetWindowText(this->m_hWndTitleLabel, text);
}
void TKTextField::SetFont(const LPCWSTR& fontName, uint32_t fontSize) const
{
	HFONT hTitleFont(::CreateFont(
		static_cast<int>(fontSize), 0, 0, 0, FW_MEDIUM,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		fontName
	));

	::SendMessage(this->m_hWndTitleLabel, WM_SETFONT, WPARAM(hTitleFont), TRUE);
}
void TKTextField::SetRect(const TKWindowFrame& frame) const
{
	//this->m_frame = TKWindowFrame(frame);

	// Redraw ±¸Çö.
}
TKWindowFrame TKTextField::GetRect() const
{
	return this->m_frame;
}