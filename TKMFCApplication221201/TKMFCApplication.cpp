#include "TKMFCApplication.h"
#include "CMainFrame.h"

BOOL TKMFCApplication::InitInstance()
{
	if (CWinApp::InitInstance() == FALSE)
		return FALSE;

	CMainFrame* pFrame = new CMainFrame();
	pFrame->Create(0, L"TKWindow");
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	this->m_pMainWnd = pFrame; //CWinApp::m_pMainWnd = pFrame;

	return TRUE;
}

TKMFCApplication app;