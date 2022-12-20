#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <ShObjIdl.h>
#include <atlbase.h>

#include "TKMainWindow.h"
#include "TKCustomMenu.h"
#include "TKWindow.h"

template<typename T>
class TKSmartPointer
{
private:
	T* p_ptr;

public:
	TKSmartPointer(T* p) : p_ptr(p) { }
	~TKSmartPointer()
	{
		if (this->p_ptr != NULL)
			this->p_ptr->Release();
	}
};

typedef struct __MONITOR_RECT
{
	std::vector<RECT> monitorRects;
	RECT combinedRect;

	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMon, HDC hdc, LPRECT lpMonitorRect, LPARAM lParam)
	{
		__MONITOR_RECT* thisRect = reinterpret_cast<__MONITOR_RECT*>(lParam);
	}

	void MonitorRects()
	{
		::SetRectEmpty(&this->combinedRect);
		::EnumDisplayMonitors(0, 0, this->MonitorEnumProc, (LPARAM)this);
	}

} TKMonitorRects;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

class TKTextField
{
private:
	TKWindow m_background;
	TKWindow m_titleLabel;
	TKWindow m_inputField;

	TKWindow GetBackgroundWindow() const
	{
		return TKWindow(
			TKWindow::PreviousWindowClass::STATIC,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			{ 20, 10, 460, 30 }
		);
	}
	TKWindow GetTitleLabel() const
	{
		return TKWindow(
			TKWindow::PreviousWindowClass::STATIC,
			L"Title: ",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT,
			{ 5, 5, 50, 20 }
		);
	}
	TKWindow GetTextField() const
	{
		return TKWindow(
			TKWindow::PreviousWindowClass::EDIT,
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			{ 55, 5, 405, 20 }
		);
	}

public:
	typedef struct __INTERNAL_WINDOW_HANDLER
	{
	public:
		__INTERNAL_WINDOW_HANDLER(HWND hWndBackground, HWND hWndTitleLabel, HWND hWndInputField) :
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

	private:
		HWND m_hWndBackground;
		HWND m_hWndTitleLabel;
		HWND m_hWndInputField;

	} WindowHandlers;

	TKTextField() : 
		m_background(this->GetBackgroundWindow()),
		m_titleLabel(this->GetTitleLabel()),
		m_inputField(this->GetTextField())
	{
	}

	WindowHandlers CreateTextField(HWND hWndParent)
	{
		HWND hWndBackground(this->m_background.InsertToParentWindow(hWndParent));
		HWND hWndTitleLabel(this->m_titleLabel.InsertToParentWindow(hWndBackground));
		
		return WindowHandlers(
			hWndBackground,
			hWndTitleLabel,
			this->m_inputField.InsertToParentWindow(hWndBackground)
		);
	}
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
	HICON windowIcon(::LoadIcon(NULL, IDI_ASTERISK));
	HCURSOR windowCursor(::LoadCursor(NULL, IDC_IBEAM));
	HBRUSH windowBackground((HBRUSH)COLOR_WINDOW);
	WNDCLASS wc{
		CS_HREDRAW | CS_VREDRAW,
		::WindowProcedure,
		0, 0,
		hInst,
		windowIcon,
		windowCursor,
		windowBackground,
		NULL,
		L"TKWindowClass"
	};
	
	if (::RegisterClass(&wc) == 0)
	{
		std::cout << "Unable to register class." << std::endl;
		return -1;
	}

	HWND hwnd(::CreateWindow(
		wc.lpszClassName,
		L"New Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		300, 200,
		500, 400,
		NULL, NULL, NULL, NULL
	));
	MSG msg{ 0 };

	while (::GetMessage(&msg, NULL, NULL, NULL))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		TKCustomMenu::SetMenu(hWnd);
		TKTextField textField;
		TKTextField::WindowHandlers handlers(textField.CreateTextField(hWnd));
		
		handlers.DefaultFontWithSize(20);
	}
	return 0;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case TKCustomMenu::TKMenuNewFile:
			::MessageBeep(MB_OK);
			::OutputDebugString(L"Action new file.\n");
			break;

		case TKCustomMenu::TKMenuExitFile:
			::DestroyWindow(hWnd);
			break;

		case TKCustomMenu::TKMenuEditTitle:
			
			break;

		default:
			::OutputDebugString(L"Else action...\n");
			break;
		}
	}
		return 0;

	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;

	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}
}