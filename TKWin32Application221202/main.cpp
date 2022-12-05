#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "TKMainWindow.h"

//struct TKUserAccount
//{
//UINT16 userId;
//const wchar_t* userName;
//const wchar_t* userDescription;
//};
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	TKUserAccount* userAccount = nullptr;
//
//	if (uMsg == WM_CREATE)
//	{
//		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
//		userAccount = reinterpret_cast<TKUserAccount*>(cs->lpCreateParams);
//
//		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)userAccount);
//	}
//	else
//	{
//		LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
//		userAccount = reinterpret_cast<TKUserAccount*>(lp);
//
//		if (userAccount != nullptr)
//		{
//			OutputDebugString(userAccount->userDescription);
//			OutputDebugString(L"\n");
//		}
//	}
//
//	switch (uMsg)
//	{
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hWnd, &ps);
//
//		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
//		EndPaint(hWnd, &ps);
//
//		return 0;
//	}
//
//	case WM_CLOSE:
//		if (MessageBox(hWnd, L"Do you want quit?", L"Notice", MB_OKCANCEL) == IDOK)
//		{
//			OutputDebugString(L"Select OK.\n");
//			DestroyWindow(hWnd);
//		}
//		else
//		{
//			OutputDebugString(L"Select CANCLE.\n");
//		}
//
//		return 0;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}

#include <ShObjIdl.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//TKMainWindow mainWindow;

	//if (mainWindow.Create(L"New Window", WS_OVERLAPPEDWINDOW) == FALSE)
	//	return 0;

	//ShowWindow(mainWindow.Window(), nCmdShow);

	//MSG msg = {};

	//while (GetMessage(&msg, NULL, 0, 0) > 0)
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (FAILED(hr))
	{
		OutputDebugString(L"Fail COM initialize.");
		return 0;
	}

	IFileOpenDialog* fileOpenDialog;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpenDialog));

	if (FAILED(hr))
	{
		OutputDebugString(L"Fail COM create instance.");
		return 0;
	}

	hr = fileOpenDialog->Show(NULL);

	if (FAILED(hr))
	{
		OutputDebugString(L"Fail to show file open dialog.");

		fileOpenDialog->Release();
		return 0;
	}

	IShellItem* item;
	hr = fileOpenDialog->GetResult(&item);

	if (FAILED(hr))
	{
		OutputDebugString(L"Fail to get result.");
		return 0;
	}

	PWSTR pszFilePath;
	hr = item->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	if (FAILED(hr))
	{
		OutputDebugString(L"Fail to get display name.");

		item->Release();
		return 0;
	}

	MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
	CoTaskMemFree(pszFilePath);
	CoUninitialize();

	return 0;
}