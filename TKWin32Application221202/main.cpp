#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <iostream>

struct TKUserAccount
{
	UINT16 userId;
	const wchar_t* userName;
	const wchar_t* userDescription;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TKUserAccount* userAccount = nullptr;

	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		userAccount = reinterpret_cast<TKUserAccount*>(cs->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)userAccount);
	}
	else 
	{
		LONG_PTR lp = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		userAccount = reinterpret_cast<TKUserAccount*>(lp);

		if (userAccount != nullptr)
		{
			OutputDebugString(userAccount->userDescription);
			OutputDebugString(L"\n");
		}
	}

	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
		EndPaint(hWnd, &ps);

		return 0;
	}
		
	case WM_CLOSE:
		if (MessageBox(hWnd, L"Do you want quit?", L"Notice", MB_OKCANCEL) == IDOK)
		{
			OutputDebugString(L"Select OK.\n");
			DestroyWindow(hWnd);
		}
		else
		{
			OutputDebugString(L"Select CANCLE.\n");
		}
			
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	wchar_t const CLASS_NAME[] = L"TKWindowClass";
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	TKUserAccount *userAccount = new(std::nothrow) TKUserAccount();
	userAccount->userId = 0;
	userAccount->userName = L"timothy-20";
	userAccount->userDescription = L"This is timothy-20's account.";

	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Learn windows programming",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		userAccount
	);

	if (hWnd == NULL)
		return 0;

	ShowWindow(hWnd, nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}