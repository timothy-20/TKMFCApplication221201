#include "TKMainWindow.h"

PCWSTR TKMainWindow::ClassName() const
{
	 return L"TKMainWindow";
}

LRESULT TKMainWindow::HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(this->_hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(this->_hwnd, &ps);
	}
	return 0;

	case WM_CLOSE:
		if (MessageBox(this->_hwnd, L"Do you want quit?", L"Notice", MB_OKCANCEL) == IDOK)
			DestroyWindow(this->_hwnd);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(this->_hwnd, umsg, wparam, lparam);
	}

	return TRUE;
}