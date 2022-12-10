#include "TKMainWindow.h"

void TKMainWindow::CalculateLayout()
{
	if (this->p_renderTarget != nullptr)
	{
		D2D1_SIZE_F size = this->p_renderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		this->m_ellipse = D2D1::Ellipse(D2D1::Point2(x, y), radius, radius);
	}
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

			if (SUCCEEDED(handleResult))
				this->CalculateLayout();
		}
	}

	return handleResult;
}

void TKMainWindow::DiscardGraphicsResources()
{
	::TKSafeRelease(&this->p_renderTarget);
	::TKSafeRelease(&this->p_brush);
}

void TKMainWindow::OnPaint()
{
	HRESULT handleResult = CreateGraphicsResources();

	if (SUCCEEDED(handleResult))
	{
		PAINTSTRUCT paint;
		
		::BeginPaint(this->m_hwnd, &paint);

		this->p_renderTarget->BeginDraw();
		this->p_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		this->p_renderTarget->FillEllipse(this->m_ellipse, this->p_brush);
		//this->p_renderTarget->DrawEllipse(this->m_ellipse, )




		handleResult = this->p_renderTarget->EndDraw();

		if (FAILED(handleResult) || handleResult == D2DERR_RECREATE_TARGET)
			this->DiscardGraphicsResources();

		::EndPaint(this->m_hwnd, &paint);
	}
}

void TKMainWindow::ReSize()
{
	if (this->p_renderTarget != nullptr)
	{
		RECT rect;

		::GetClientRect(this->m_hwnd, &rect);

		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

		this->p_renderTarget->Resize(size);
		this->CalculateLayout();
		::InvalidateRect(this->m_hwnd, &rect, FALSE);
	}
}

LRESULT TKMainWindow::HandleMessage(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->p_factory)))
			return -1;

		return 0;

	case WM_PAINT:
	{
		/*PAINTSTRUCT ps;
		HDC hdc = BeginPaint(this->m_hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(this->m_hwnd, &ps);*/

		this->OnPaint();
	}
	return 0;

	case WM_SIZE:
		this->ReSize();
		return 0;

	case WM_CLOSE:
		if (MessageBox(this->m_hwnd, L"Do you want quit?", L"Notice", MB_OKCANCEL) == IDOK)
			::DestroyWindow(this->m_hwnd);

		return 0;

	case WM_DESTROY:
		this->DiscardGraphicsResources();
		::TKSafeRelease(&this->p_factory);
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(this->m_hwnd, umsg, wparam, lparam);
}