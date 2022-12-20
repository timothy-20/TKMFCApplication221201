#include "TKCustomMenu.h"

TKCustomMenu::TKCustomMenu() :
	m_menu(::CreateMenu()),
	m_textfield()
{
	::AppendMenu(this->m_menu, MF_POPUP, (UINT_PTR)this->GetFileMenu(), L"File");
	::AppendMenu(this->m_menu, MF_STRING, TKMenuEditTitle, L"Edit");
}

HMENU TKCustomMenu::GetFileMenu()
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

void TKCustomMenu::SetMenu(HWND hWnd)
{
	TKCustomMenu menu;

	::SetMenu(hWnd, menu.m_menu);
}