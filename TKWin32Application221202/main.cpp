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

LRESULT CALLBACK WinProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
	HICON windowIcon(::LoadIcon(NULL, IDI_ASTERISK));
	WNDCLASSEX wcex{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW,
		::WinProcedure,
		0,
		0,
		hInst,
		windowIcon,
		::LoadCursor(NULL, IDC_IBEAM),
		(HBRUSH)(COLOR_BACKGROUND + 1),
		L"Main Window",
		L"TKMainWindow",
		windowIcon
	};
	
	if (::RegisterClassEx(&wcex) == 0)
		return -1;

	//RECT clientRect{};
	//::getscreen
	//if (::GetClientRect(NULL, &clientRect))
	//{
	//	::CreateWindowEx(
	//		WS_EX_OVERLAPPEDWINDOW,
	//		wcex.lpszClassName,
	//		L"New Window",
	//		WS_OVERLAPPEDWINDOW,
	//		(clientRect.right / 2), (clientRect.bottom / 2),
	//		300, 150,
	//		NULL, NULL, NULL, NULL);

	//}

	return 0;
}

LRESULT CALLBACK WinProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}