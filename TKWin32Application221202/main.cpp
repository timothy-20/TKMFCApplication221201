#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <iostream>
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

LRESULT CALLBACK WinProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
	WNDCLASS wc{
		CS_DROPSHADOW,
		::WinProcedure,
		0,
		0,
		hInst,
		NULL,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)COLOR_WINDOW,
		L"Main Window",
		L"TKMainWindow"
	};
	return 0;
}

LRESULT CALLBACK WinProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}