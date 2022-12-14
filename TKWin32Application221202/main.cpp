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
	HMENU m_textfield;

public:
	enum : char
	{
		TKMenuNewFile = 'N',
		TKMenuExitFile = 'E',
		TKMenuEditTitle = 'C'
	};

public:
	TKCustomMenu() :
		m_menu(::CreateMenu()),
		m_textfield()
	{
		::AppendMenu(this->m_menu, MF_POPUP, (UINT_PTR)this->GetFileMenu(), L"File");
		::AppendMenu(this->m_menu, MF_STRING, TKMenuEditTitle, L"Edit");
	}

	~TKCustomMenu() {};

	HMENU GetFileMenu() 
	{
		HMENU fileOpenSubMenu(::CreateMenu());
		
		::AppendMenu(fileOpenSubMenu, MF_STRING, NULL, L"Projects");
		::AppendMenu(fileOpenSubMenu, MF_STRING, NULL, L"Folder");
	
		HMENU fileMenu(::CreateMenu());

		::AppendMenu(fileMenu, MF_STRING, TKMenuNewFile, L"New File");
		::AppendMenu(fileMenu, MF_POPUP, (UINT_PTR)fileOpenSubMenu, L"Open");
		::AppendMenu(fileMenu, MF_SEPARATOR, NULL, NULL);
		::AppendMenu(fileMenu, MF_STRING, TKMenuExitFile, L"Exit");

		return fileMenu;
	}

	static void SetMenu(HWND hWnd)
	{
		TKCustomMenu menu;
		
		::SetMenu(hWnd, menu.m_menu);
	}

	static std::map<std::string, HWND> GetTextfieldLabel(HWND hWndParent)
	{
		HWND hWndTextfield(::CreateWindow(
			NULL,
			NULL,
			WS_CHILD | WS_BORDER,
			20, 10, 460, 30,
			hWndParent,
			NULL, NULL, NULL
		));
		HWND hWndStatic(::CreateWindow(
			L"static",
			L"Title : ",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT,
			5, 5, 50, 20,
			hWndTextfield,
			NULL, NULL, NULL
		));
		HFONT hFontStatic(::CreateFont(
			24, 0, 0, 0, FW_MEDIUM,
			FALSE, FALSE, FALSE,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Arial"
		));

		::SendMessage(hWndStatic, WM_SETFONT, WPARAM(hFontStatic), TRUE);

		HWND hWndEdit(::CreateWindow(
			L"edit",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			55, 5, 405, 20,
			hWndTextfield,
			NULL, NULL, NULL
		));

		return {};
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
		TKCustomMenu::SetMenu(hWnd);
		
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