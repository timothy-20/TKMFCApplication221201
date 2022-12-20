#pragma once
#include <Windows.h>

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
	TKCustomMenu();
	~TKCustomMenu() = default;

	// Utils
	HMENU GetFileMenu();
	static void SetMenu(HWND hWnd);
};