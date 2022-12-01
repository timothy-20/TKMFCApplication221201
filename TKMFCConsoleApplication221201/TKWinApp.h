#pragma once
typedef int BOOL;
#define TRUE 1
#define FALSE 0

class TKWinApp
{
public:
	TKWinApp();
	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();
};

TKWinApp* ::AFTGetWinApp();

