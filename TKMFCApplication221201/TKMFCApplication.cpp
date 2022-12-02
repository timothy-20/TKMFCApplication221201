#include "TKMFCApplication.h"
#include "CMainFrame.h"
#include "TKMainWindow.h"

BOOL TKMFCApplication::InitInstance()
{
	if (CWinApp::InitInstance() == FALSE)
		return FALSE;

	CMainFrame* pFrame = new CMainFrame();
	pFrame->Create(0, L"TKWindow");
	/*pFrame->Create(
		0, 
		L"TKWindow", 
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX,
		CRect(0 , 0, 300, 300), 0, 0, 0, 0);*/
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	this->m_pMainWnd = pFrame; //CWinApp::m_pMainWnd = pFrame;

	//CString className = AfxRegisterWndClass(
	//	CS_HREDRAW | CS_VREDRAW, //horizontal, vertical size redraw
	//	LoadStandardCursor(IDC_APPSTARTING), //window cursor option
	//	(HBRUSH)::GetStockObject(WHITE_BRUSH), //
	//	LoadStandardIcon(IDI_APPLICATION) // window icon option
	//);
	//TKMainWindow* mainWindow = new TKMainWindow();
	//mainWindow->CreateEx(0, className, L"Hello, C Window!", WS_OVERLAPPEDWINDOW, CRect(0, 0, 300, 300), 0, 0);
	//mainWindow->ShowWindow(SW_SHOW);
	//this->m_pMainWnd = mainWindow;

	return TRUE;
}

TKMFCApplication app;