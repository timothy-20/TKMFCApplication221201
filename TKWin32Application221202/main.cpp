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
#include <atlbase.h>

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

void MakeshiftFuntionScope()
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

	if (SUCCEEDED(hr))
	{
		CComPtr<IFileOpenDialog> fileOpenDialog = NULL;
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpenDialog));

		if (SUCCEEDED(hr))
		{
			hr = fileOpenDialog->Show(NULL);

			if (SUCCEEDED(hr))
			{
				CComPtr<IShellItem> item = NULL;
				hr = fileOpenDialog->GetResult(&item);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = item->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}

					//item->Release();
					//::TKSafeRelease(&item);
				}
			}

			//fileOpenDialog->Release();
			//::TKSafeRelease(&fileOpenDialog);
		}

		CoUninitialize();
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	TKMainWindow mainWindow;

	if (mainWindow.Create(L"Circle", WS_OVERLAPPEDWINDOW) == FALSE)
		return 0;

	::ShowWindow(mainWindow.Window(), nCmdShow);

	MSG msg = {};

	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 0;
}