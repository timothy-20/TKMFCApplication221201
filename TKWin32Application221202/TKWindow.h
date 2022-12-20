#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

typedef struct __WINDOW_FRAME
{
	int x;
	int y;
	int width;
	int height;

	__WINDOW_FRAME() : x(0), y(0), width(0), height(0) { }
	__WINDOW_FRAME(std::initializer_list<int> list) : //불필요하지만, initializer_list 구현 연습.
		x(*list.begin()),
		y(*(list.begin() + 1)),
		width(*(list.begin() + 2)),
		height(*(list.begin() + 3))
	{
	}

} TKWindowFrame;

class TKWindow;
using TKWindowContainer = std::vector<std::unique_ptr<TKWindow>>;

//TKWindow 내부에서 subWindow에 대한 관리가 가능한 기능을 추가할 것.
//TKWindow 객체에서 frame 조정 시 resize에 대한 지원 추가할 것.
class TKWindow
{
private:
	LPCWSTR m_className;
	LPCWSTR m_windowName;
	DWORD m_style;
	TKWindowFrame m_frame;
	HMENU m_hMenu;
	HINSTANCE m_hInstance;
	LPVOID m_lParam;
	TKWindowContainer m_subwindows;

public:
	enum class PreviousWindowClass : unsigned short
	{
		NONE = 0,
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

	TKWindow(
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
		m_lParam(lParam),
		m_subwindows(TKWindowContainer()) { }
	TKWindow(const TKWindow& crWindow) :
		m_className(crWindow.m_className),
		m_windowName(crWindow.m_windowName),
		m_style(crWindow.m_style),
		m_frame(crWindow.m_frame),
		m_hMenu(crWindow.m_hMenu),
		m_hInstance(crWindow.m_hInstance),
		m_lParam(crWindow.m_lParam) { }

	// Utils
	LPCWSTR GetClassNameW(PreviousWindowClass windowClassName);
	HWND InsertToParentWindow(HWND hWndParent);
	void AddSubwindow(TKWindow window);
	void SetFrame(TKWindowFrame frame);
	int GetOriginX() const { return this->m_frame.x; }
	int GetOriginY() const { return this->m_frame.y; }
	int GetSizeWidth() const { return this->m_frame.width; }
	int GetSizeHeight() const { return this->m_frame.height; }
	static TKWindow GetDefaultChildWindow();
};