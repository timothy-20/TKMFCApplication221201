#pragma once
#include "TKBaseWindow.h"
#include "TKClockScene.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

template<typename T>
void TKSafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class TKMainWindow : public TKBaseWindow<TKMainWindow>
{
private:
	HANDLE m_timer;
	TKClockScene m_clockScene;

	BOOL InitializeTimer();

public:
	TKMainWindow() : m_timer(NULL) { }
	PCWSTR ClassName() const { return L"TKMainWindow"; }
	LRESULT HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam);
	void WaitTimer();
};