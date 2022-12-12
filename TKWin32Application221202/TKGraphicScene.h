#pragma once
#include<Windows.h>
#include<atlacc.h>
#include<d2d1.h>

class TKGraphicScene
{
protected:
	CComPtr<ID2D1Factory> m_factory;
	CComPtr<ID2D1HwndRenderTarget> m_renderTarget;
	float m_scaleX;
	float m_scaleY;

	virtual HRESULT CreateDeviceIndependentResources() = 0;
	virtual void DiscardDeviceIndependentResources() = 0;
	virtual HRESULT CreateDeviceDependentResources() = 0;
	virtual void DiscardDeviceDependentResources() = 0;
	virtual void CalculateLayout() = 0;
	virtual void RenderScene() = 0;

	HRESULT CreateGraphicResources(HWND hwnd)
	{
		HRESULT hr(S_OK);

		if (this->m_renderTarget == nullptr)
		{
			RECT rect{};

			::GetClientRect(hwnd, &rect);

			hr = this->m_factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rect.right, rect.bottom)),
				&this->m_renderTarget
			);

			if (SUCCEEDED(hr))
			{
				hr = this->CreateDeviceDependentResources();

				if (SUCCEEDED(hr))
					this->CalculateLayout();
			}
		}

		return hr;
	}

	template<typename T>
	T PixelToDipX(T pixels) const { return static_cast<T>(pixels / this->m_scaleX); }

	template<typename T>
	T PixelToDipY(T pixels) const { return static_cast<T>(pixels / this->m_scaleY); }

public:
	TKGraphicScene() : m_scaleX(1.0f), m_scaleY(1.0f) {}
	virtual ~TKGraphicScene() {}

	HRESULT Initialize()
	{
		HRESULT hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->m_factory);

		if (SUCCEEDED(hr))
			this->CreateDeviceIndependentResources();

		return hr;
	}

	void Render(HWND hwnd)
	{
		if (FAILED(this->CreateGraphicResources(hwnd)))
			return;

		if (this->m_renderTarget != nullptr)
		{
			this->m_renderTarget->BeginDraw();
			this->RenderScene();

			if (this->m_renderTarget->EndDraw() == D2DERR_RECREATE_TARGET)
			{
				this->DiscardDeviceDependentResources();
				this->m_renderTarget.Release();
			}
		}
	}

	HRESULT Resize(int x, int y)
	{
		HRESULT hr(S_OK);

		if (this->m_renderTarget != nullptr)
		{
			hr = this->m_renderTarget->Resize(D2D1::SizeU(x, y));

			if (SUCCEEDED(hr))
				this->CalculateLayout();
		}

		return hr;
	}

	void CleanUp()
	{
		this->DiscardDeviceDependentResources();
		this->DiscardDeviceIndependentResources();
	}
};

