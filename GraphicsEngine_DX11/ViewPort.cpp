#include "pch.h"
#include "ViewPort.h"

void ViewPort::Initialize(const Vector2& leftTop, const int& screenWidth, const int& screenHeight)
{
	_leftTop = leftTop;

	_viewport.TopLeftX = _leftTop.x;
	_viewport.TopLeftY = _leftTop.y;
	_viewport.Width = static_cast<float>(screenWidth);
	_viewport.Height = static_cast<float>(screenHeight);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}

void ViewPort::OnResize(const int& screenWidth, const int& screenHeight)
{
	_viewport.TopLeftX = _leftTop.x;
	_viewport.TopLeftY = _leftTop.y;
	_viewport.Width = static_cast<float>(screenWidth);
	_viewport.Height = static_cast<float>(screenHeight);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}

void ViewPort::OnResize(const Vector2 leftTop, const int& screenWidth, const int& screenHeight)
{
	Initialize(leftTop, screenWidth, screenHeight);
}

void ViewPort::SetViewPort(ComPtr<ID3D11DeviceContext> deviceContext)
{
	deviceContext->RSSetViewports(1, &_viewport);
}

void ViewPort::Release()
{

}
