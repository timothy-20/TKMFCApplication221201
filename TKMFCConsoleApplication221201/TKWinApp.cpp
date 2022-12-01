#include "TKWinApp.h"

TKWinApp* globalApp = 0;

TKWinApp* ::AFTGetWinApp()
{
	return globalApp;
}

TKWinApp::TKWinApp() 
{
	globalApp = this;
}

BOOL TKWinApp::InitInstance() 
{
	return TRUE;
}

int TKWinApp::Run() 
{
	return 0;
}

int TKWinApp::ExitInstance() 
{
	return 0;
}