#include "TKMainWindow.h"

BOOL TKMainWindow::InitializeTimer()
{
	this->m_timer = ::CreateWaitableTimer(nullptr, FALSE, nullptr);

	if (this->m_timer == nullptr)
		return FALSE;

	LARGE_INTEGER li{ 0 };

	if (::SetWaitableTimer(this->m_timer, &li, (1000 / 60), NULL, NULL, FALSE) == FALSE)
	{
		::CloseHandle(this->m_timer);
		this->m_timer = nullptr;

		return FALSE;
	}

	return TRUE;
}

void TKMainWindow::WaitTimer()
{
	if (::MsgWaitForMultipleObjects(1, &this->m_timer, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
		::InvalidateRect(this->m_hwnd, NULL, FALSE);
}

LRESULT TKMainWindow::HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_CREATE:
		if (FAILED(this->m_clockScene.Initialize()) || this->InitializeTimer() == FALSE)
			return -1;

		return 0;

	case WM_PAINT:
	case WM_DISPLAYCHANGE:
	{
		PAINTSTRUCT ps{};

		::BeginPaint(this->m_hwnd, &ps);
		this->m_clockScene.Render(this->m_hwnd);
		::EndPaint(this->m_hwnd, &ps);
	}
	return 0;

	case WM_SIZE:
		this->m_clockScene.Resize((int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam));
		::InvalidateRect(this->m_hwnd, NULL, FALSE);
		return 0;

	case WM_ERASEBKGND:
		return 1;

	case WM_CLOSE:
		if (MessageBox(this->m_hwnd, L"Do you want quit?", L"Notice", MB_OKCANCEL) == IDOK)
			::DestroyWindow(this->m_hwnd);

		return 0;

	case WM_DESTROY:
		::CloseHandle(this->m_timer);
		this->m_clockScene.CleanUp();
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(this->m_hwnd, umsg, wparam, lparam);
}