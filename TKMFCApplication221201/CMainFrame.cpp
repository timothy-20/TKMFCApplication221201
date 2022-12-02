#include "CMainFrame.h"

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (CFrameWnd::PreCreateWindow(cs) == FALSE)
		return FALSE;

	cs.style = cs.style & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
	cs.dwExStyle = cs.dwExStyle | WS_EX_TOPMOST;
	cs.x = 0;
	cs.y = 0;
	cs.cx = 200;
	cs.cy = 200;
	cs.lpszClass = AfxRegisterWndClass(
		0,
		(HCURSOR)::LoadCursor(0, IDC_IBEAM),
		(HBRUSH)::GetStockObject(WHITE_BRUSH),
		(HICON)::LoadIcon(0, IDI_APPLICATION));

	return TRUE;
}