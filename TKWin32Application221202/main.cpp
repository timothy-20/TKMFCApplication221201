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
#include "TKWindow.h"
#include "TKCustomMenu.h"
#include "TKTextField.h"

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

	RECT rect{ 0, 0, 640, 480 };

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	int adjustWidth(rect.right - rect.left);
	int adjustHeight(rect.bottom - rect.top);

	HWND hwnd(::CreateWindow(
		wc.lpszClassName,
		L"New Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		adjustWidth, 
		adjustHeight,
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
		TKTextField titleTextField({ 10, 20, 620, 50 });

		titleTextField.CreateTextField(hWnd);
		titleTextField.SetTitleText(L"title: ");

		TKTextField subtitleTextField({ 10, 80, 620, 50 });

		subtitleTextField.CreateTextField(hWnd);
		subtitleTextField.SetTitleText(L"subtitle: ");

		TKTextField contentTextField({ 10, 140, 620, 320 });

		contentTextField.CreateTextField(hWnd);
		contentTextField.SetTitleText(L"description: ");
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