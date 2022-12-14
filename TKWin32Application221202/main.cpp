#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ShObjIdl.h>
#include <atlbase.h>

#include "TKMainWindow.h"

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

class TKCustomMenu
{
private:
	HMENU m_menu;
	HMENU m_fileMenu;

public:
	enum : char
	{
		TKFileMenuNew = 'N',
		TKFileMenuExit = 'E'
	};

public:
	TKCustomMenu() :
		m_menu(::CreateMenu()),
		m_fileMenu(this->GetFileMenu()) 
	{
		::AppendMenu(this->m_menu, MF_POPUP, (UINT_PTR)this->m_fileMenu, L"File");
	}

	~TKCustomMenu() {};

	HMENU GetFileMenu() 
	{
		HMENU fileOpenSubMenu(::CreateMenu());
		
		::AppendMenu(fileOpenSubMenu, MF_STRING, NULL, L"Projects");
		::AppendMenu(fileOpenSubMenu, MF_STRING, NULL, L"Folder");
	
		HMENU fileMenu(::CreateMenu());

		::AppendMenu(fileMenu, MF_STRING, TKFileMenuNew, L"New File");
		::AppendMenu(fileMenu, MF_POPUP, (UINT_PTR)fileOpenSubMenu, L"Open");
		::AppendMenu(fileMenu, MF_SEPARATOR, NULL, NULL);
		::AppendMenu(fileMenu, MF_STRING, TKFileMenuExit, L"Exit");

		return fileMenu;
	}

	static void SetMenu(HWND hWnd)
	{
		TKCustomMenu menu;
		
		::SetMenu(hWnd, menu.m_menu);
	}
};

HWND SetControlWindow(HWND hWndParent)
{
	HWND hWndStatic(::CreateWindow(
		L"static",
		L"Enter text here :",
		WS_CHILD | WS_VISIBLE | WS_BORDER || SS_CENTER,
		100, 100,
		300, 50,
		hWndParent,
		NULL, NULL, NULL, NULL
	));
	HWND hWndEdit(::CreateWindow(
		L"edit",
		L"",
	));

	return hWndStatic;
}


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
		TKCustomMenu::SetMenu(hWnd);
		::SetControlWindow(hWnd);
		return 0;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case TKCustomMenu::TKFileMenuNew:
			::MessageBeep(MB_OK);
			::OutputDebugString(L"Action new file.\n");
			break;

		case TKCustomMenu::TKFileMenuExit:
			::DestroyWindow(hWnd);
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