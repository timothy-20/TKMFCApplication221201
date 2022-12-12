#include "TKClockScene.h"

HRESULT TKClockScene::CreateDeviceDependentResources()
{
	HRESULT hr = this->m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(1.0f, 1.0f, 0),
		D2D1::BrushProperties(),
		&this->m_Fill
	);

	if (SUCCEEDED(hr))
	{
		hr = this->m_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0, 0, 0),
			D2D1::BrushProperties(),
			&this->m_Stroke
		);
	}

	return hr;
}

void TKClockScene::RenderScene()
{
	this->m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
	this->m_renderTarget->FillEllipse(this->m_ellipse, this->m_Fill);
	this->m_renderTarget->DrawEllipse(this->m_ellipse, this->m_Stroke);

	for (DWORD i = 0; i < 12; i++)
		this->m_renderTarget->DrawLine(this->m_Ticks[i * 2], this->m_Ticks[i * 2 + 1], this->m_Stroke, 2.0f);

	SYSTEMTIME time;

	::GetLocalTime(&time);

	const float hourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
	const float minuteAngle = (360.0f / 60) * (time.wMinute);
	const float secondAngle = (360.0f / 60) * time.wSecond + (360.0f / 60000) * time.wMilliseconds;

	this->DrawClockHand(0.6f, hourAngle, 6.0f);
	this->DrawClockHand(0.7f, minuteAngle, 4.0f);
	this->DrawClockHand(0.85f, secondAngle, 1.0f);
	this->m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void TKClockScene::DrawClockHand(float handLength, float angle, float strokeWidth)
{
	this->m_renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, this->m_ellipse.point));
	D2D_POINT_2F endPoint = D2D1::Point2F(
		this->m_ellipse.point.x,
		this->m_ellipse.point.y - (this->m_ellipse.radiusY * handLength)
	);

	this->m_renderTarget->DrawLine(this->m_ellipse.point, endPoint, this->m_Stroke, strokeWidth);
}

void TKClockScene::CalculateLayout()
{
	D2D1_SIZE_F size = this->m_renderTarget->GetSize();
	const float x = size.width / 2.0f;
	const float y = size.height / 2.0f;
	const float radius = min(x, y);
	this->m_ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	D2D_POINT_2F point1 = D2D1::Point2F(
		this->m_ellipse.point.x,
		this->m_ellipse.point.y - (this->m_ellipse.radiusY * 0.9f)
	);
	D2D1_POINT_2F point2 = D2D1::Point2F(
		this->m_ellipse.point.x,
		this->m_ellipse.point.y - this->m_ellipse.radiusY
	);
	
	for (DWORD i = 0; i < 12; i++)
	{
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Rotation((360.0f / 12) * i, this->m_ellipse.point);
		this->m_Ticks[i * 2] = matrix.TransformPoint(point1);
		this->m_Ticks[i * 2 + 1] = matrix.TransformPoint(point2);
	}
}

void TKClockScene::DiscardDeviceDependentResources()
{
	this->m_Fill.Release();
	this->m_Stroke.Release();
}