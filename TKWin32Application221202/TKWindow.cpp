#include "TKWindow.h"

LPCWSTR TKWindow::GetClassNameW(PreviousWindowClass windowClassName)
{
	switch (windowClassName)
	{
	case PreviousWindowClass::BUTTON: return L"button";
	case PreviousWindowClass::COMBOBOX: return L"combobox";
	case PreviousWindowClass::EDIT: return L"edit";
	case PreviousWindowClass::LISTBOX: return L"listbox";
	case PreviousWindowClass::MDICLIENT: return L"mdiclient";
	case PreviousWindowClass::RICHEDIT: return L"richedit";
	case PreviousWindowClass::RICHEDIT_CLASS: return L"richedit_class";
	case PreviousWindowClass::SCROLLBAR: return L"scrollbar";
	case PreviousWindowClass::STATIC: return L"static";
	default: return NULL;
	}
}

HWND TKWindow::InsertToParentWindow(HWND hWndParent)
{
	return ::CreateWindowW(
		this->m_className,
		this->m_windowName,
		this->m_style,
		this->m_frame.x,
		this->m_frame.y,
		this->m_frame.width,
		this->m_frame.height,
		hWndParent,
		this->m_hMenu,
		this->m_hInstance,
		this->m_lParam
	);
}

void TKWindow::AddSubwindow(TKWindow window)
{
	std::unique_ptr<TKWindow> upWindow(new TKWindow(window));
	//this->m_subwindows.push_back()
}

void TKWindow::SetFrame(TKWindowFrame frame)
{

}

TKWindow TKWindow::GetDefaultChildWindow()
{
	return TKWindow(
		TKWindow::PreviousWindowClass::STATIC,
		NULL,
		WS_CHILD | WS_VISIBLE,
		TKWindowFrame{ 0, 0, 100, 100 }
	);
}