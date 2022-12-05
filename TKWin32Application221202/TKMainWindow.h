#pragma once
#include "TKBaseWindow.h"

class TKMainWindow : public TKBaseWindow<TKMainWindow>
{
public:
	PCWSTR ClassName() const;
	LRESULT HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam);
};

