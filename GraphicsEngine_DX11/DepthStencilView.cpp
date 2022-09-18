#include "pch.h"
#include "DepthStencilView.h"

void DepthStencilView::Initialize(ComPtr<ID3D11Device> device, int width, int height, bool enable4xMsaa)
{
	// 깊이 버퍼는 그냥 깊이 정보를 담는 2차원 텍스처이다.
	// D3D11_TEXTURE2D_DESC 구조체를 채우고 CreateTexture2D 메서드를 호출해야한다.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;								// 밉맵 수준의 갯수. 깊이 스텐실 버퍼를 위한 텍스쳐는 하나만 있으면 돼
	depthBufferDesc.ArraySize = 1;								// 텍스처 배열의 텍스처 갯수. 얘도 하나만 필요
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// 텍셀 형식

	_enable4xMsaa = enable4xMsaa;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (_enable4xMsaa)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = 0;
	}
	// No MSAA
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, 0, _depthStencilBuffer.GetAddressOf());

	// D3D가 depth buffer을 depth stencil texture로 사용한다는 사실을 알도록 스텐실 description을 작성한다.
	// 뎁스/스텐실뷰 desc 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// depth stencil view description 작성 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	if (_enable4xMsaa)
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	else
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// 뎁스/스텐실뷰 생성
	device->CreateDepthStencilView(_depthStencilBuffer.Get(), 0, _depthStencilView.GetAddressOf());
}

void DepthStencilView::InitializeShadowMap(ComPtr<ID3D11Device> device, int width, int height, bool enable4xMsaa)
{
	_isShadow = true;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;								// 밉맵 수준의 갯수. 깊이 스텐실 버퍼를 위한 텍스쳐는 하나만 있으면 돼
	depthBufferDesc.ArraySize = 1;								// 텍스처 배열의 텍스처 갯수. 얘도 하나만 필요
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;		// 다른점
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;

	_enable4xMsaa = enable4xMsaa;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (_enable4xMsaa)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = 0;
	}
	// No MSAA
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;	// 다른점
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, nullptr, _depthStencilBuffer.GetAddressOf());

	// D3D가 depth buffer을 depth stencil texture로 사용한다는 사실을 알도록 스텐실 description을 작성한다.
	// 뎁스/스텐실뷰 desc 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// depth stencil view description 작성 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (_enable4xMsaa)
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	else
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// 뎁스/스텐실뷰 생성
	device->CreateDepthStencilView(_depthStencilBuffer.Get(), 0, _depthStencilView.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = depthBufferDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(_depthStencilBuffer.Get(), &srvDesc, _shaderResourceView.GetAddressOf());
}

void DepthStencilView::OnResize(ComPtr<ID3D11Device> device, int width, int height)
{
	Release();

	if(_isShadow)
		InitializeShadowMap(device, width, height, _enable4xMsaa);
	else
		Initialize(device, width, height, _enable4xMsaa);
}

void DepthStencilView::Release()
{
	_depthStencilBuffer.ReleaseAndGetAddressOf();

	_depthStencilView.ReleaseAndGetAddressOf();
}

void DepthStencilView::ClearDepthStencilView(ComPtr<ID3D11DeviceContext> deviceContext)
{
	deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
