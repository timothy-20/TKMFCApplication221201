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
	//std::map<>

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
};

typedef struct __WINDOW_FRAME
{
	int x;
	int y;
	int width;
	int height;

	__WINDOW_FRAME() : x(0), y(0), width(0), height(0) { }

	__WINDOW_FRAME(std::initializer_list<int> list) : //불필요하지만, initializer_list 구현을 연습해보고 싶었습니다.
		x(*list.begin()),
		y(*(list.begin() + 1)),
		width(*(list.begin() + 2)),
		height(*(list.begin() + 3))
	{
	}

} TKWindowFrame;

typedef struct __WINDOW_OBJECT
{
public:
	enum class PreviousWindowClass : short
	{
		BUTTON = 101,
		COMBOBOX,
		EDIT,
		LISTBOX,
		MDICLIENT,
		RICHEDIT,
		RICHEDIT_CLASS,
		SCROLLBAR,
		STATIC
	};

	__WINDOW_OBJECT(
		PreviousWindowClass windowClassType,
		LPCWCHAR windowName,
		DWORD style,
		TKWindowFrame frame,
		HMENU hMenu = NULL,
		HINSTANCE hInstance = NULL,
		LPVOID lParam = NULL) :
		m_className(this->GetClassNameW(windowClassType)),
		m_windowName(windowName),
		m_style(style),
		m_frame(frame),
		m_hMenu(hMenu),
		m_hInstance(hInstance),
		m_lParam(lParam) 
	{
	}

	LPCWSTR GetClassNameW(PreviousWindowClass windowClassName)
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
		default: return L"";
		}
	}

	static __WINDOW_OBJECT GetDefaultChildWindow()
	{
		return __WINDOW_OBJECT(
			__WINDOW_OBJECT::PreviousWindowClass::STATIC,
			NULL,
			WS_CHILD | WS_VISIBLE, 
			TKWindowFrame{ 0, 0, 100, 100 });
	}

	void InsertToParentWindow(HWND hWndParent)
	{
		::CreateWindowW(
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
	void SetFrame(TKWindowFrame frame)
	{

	}
	int GetOriginX() const { return this->m_frame.x; }
	int GetOriginY() const { return this->m_frame.y; }
	int GetSizeWidth() const { return this->m_frame.width; }
	int GetSizeHeight() const { return this->m_frame.height; }

private:
	LPCWSTR m_className;
	LPCWSTR m_windowName;
	DWORD m_style;
	TKWindowFrame m_frame;
	HMENU m_hMenu;
	HINSTANCE m_hInstance;
	LPVOID m_lParam;

} TKWindowObject;

//class TKCustomTextField
//{
//private:
//	TKWindowObject m_background;
//	TKWindowObject m_titleLabel;
//	TKWindowObject m_textField;
//
//	HWND CreateBackgroundWindow(HWND hWndParent) const
//	{
//		return ::CreateWindow(
//			NULL,
//			NULL,
//			WS_CHILD | WS_BORDER,
//			20, 10, 460, 30,
//			hWndParent,
//			NULL, NULL, NULL
//		);
//	}
//
//	HWND CreateTitleLabel(HWND hWndParent) const
//	{
//		HWND hWndTitleLabel(::CreateWindow(
//			L"static",
//			L"Title : ",
//			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT,
//			5, 5, 50, 20,
//			hWndParent,
//			NULL, NULL, NULL
//		));
//		HFONT hTitleFont(::CreateFont(
//			24, 0, 0, 0, FW_MEDIUM,
//			FALSE, FALSE, FALSE,
//			ANSI_CHARSET,
//			OUT_DEFAULT_PRECIS,
//			CLIP_DEFAULT_PRECIS,
//			DEFAULT_QUALITY,
//			DEFAULT_PITCH | FF_SWISS,
//			L"Arial"
//		));
//
//		::SendMessage(hWndTitleLabel, WM_SETFONT, WPARAM(hTitleFont), TRUE);
//
//		return hWndTitleLabel;
//	}
//
//	HWND CreateTextField(HWND hWndParent) const
//	{
//		HWND hWndTextField(::CreateWindow(
//			L"edit",
//			L"",
//			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
//			55, 5, 405, 20,
//			hWndParent,
//			NULL, NULL, NULL
//		));
//
//		return hWndTextField;
//	}
//
//public:
//	TKCustomTextField(HWND hWndParent) :
//		m_hWndBackground(this->CreateBackgroundWindow(hWndParent)),
//		m_hWndTitleLabel(this->CreateTitleLabel(this->m_hWndBackground)),
//		m_hWndTextField(this->CreateTextField(this->m_hWndBackground))
//	{
//
//	}
//
//};

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
		TKWindowObject customWindow(
			TKWindowObject::PreviousWindowClass::STATIC,
			L"Test Window",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			{ 10, 10, 100, 100 }
		);

		customWindow.InsertToParentWindow(hWnd);
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