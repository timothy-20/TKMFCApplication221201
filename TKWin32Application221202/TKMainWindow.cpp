#include "TKMainWindow.h"

void TKMainWindow::CalculateLayout()
{
	
}

HRESULT TKMainWindow::CreateGraphicsResources()
{
	HRESULT handleResult = S_OK;

	if (this->p_renderTarget == NULL)
	{
		RECT rect = {};

		::GetClientRect(this->m_hwnd, &rect);

		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		handleResult = this->p_factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(this->m_hwnd, size),
			&p_renderTarget
		);

		if (SUCCEEDED(handleResult))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0F, 1.0F, 0.0F);
			handleResult = this->p_renderTarget->CreateSolidColorBrush(color, &this->p_brush);

		}

	}

	return handleResult;
}

LRESULT TKMainWindow::HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(this->m_hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(this->m_hwnd, &ps);
	}
	return 0;

	case WM_CLOSE:
		if (MessageBox(this->m_hwnd, L"Do you want quit?", L"Notice", MB_OKCANCEL) == IDOK)
			DestroyWindow(this->m_hwnd);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(this->m_hwnd, umsg, wparam, lparam);
	}

	return TRUE;
}