#pragma once
#include <Windows.h>

template<typename DERIVED_TYPE>
class TKBaseWindow
{
protected:
	HWND m_hwnd;

protected:
	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam) = 0;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		DERIVED_TYPE* self = nullptr;

		if (umsg == WM_NCCREATE)
		{
			CREATESTRUCT* cs = (CREATESTRUCT*)lparam;
			self = (DERIVED_TYPE*)cs->lpCreateParams;

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);

			self->_hwnd = hwnd;
		}
		else
		{
			self = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (self)
		{
			return self->HandleMessage(umsg, wparam, lparam);
		}

		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

public:
	TKBaseWindow() : m_hwnd(NULL) { }
	HWND Window() const { return this->m_hwnd; }
	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT,
		HWND hwndParent = 0,
		HMENU hmenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = this->WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = this->ClassName();

		RegisterClass(&wc);

		this->m_hwnd = CreateWindowEx(dwExStyle, this->ClassName(), lpWindowName, dwStyle, x, y, width, height, hwndParent, hmenu, GetModuleHandle(NULL), this);
		
		return (this->m_hwnd ? TRUE : FALSE);
	}
};

