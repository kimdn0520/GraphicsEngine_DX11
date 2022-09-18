#include "pch.h"
#include "DepthStencilView.h"

void DepthStencilView::Initialize(ComPtr<ID3D11Device> device, int width, int height, bool enable4xMsaa)
{
	// ���� ���۴� �׳� ���� ������ ��� 2���� �ؽ�ó�̴�.
	// D3D11_TEXTURE2D_DESC ����ü�� ä��� CreateTexture2D �޼��带 ȣ���ؾ��Ѵ�.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;								// �Ӹ� ������ ����. ���� ���ٽ� ���۸� ���� �ؽ��Ĵ� �ϳ��� ������ ��
	depthBufferDesc.ArraySize = 1;								// �ؽ�ó �迭�� �ؽ�ó ����. �굵 �ϳ��� �ʿ�
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// �ؼ� ����

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

	// D3D�� depth buffer�� depth stencil texture�� ����Ѵٴ� ����� �˵��� ���ٽ� description�� �ۼ��Ѵ�.
	// ����/���ٽǺ� desc �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// depth stencil view description �ۼ� 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	if (_enable4xMsaa)
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	else
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// ����/���ٽǺ� ����
	device->CreateDepthStencilView(_depthStencilBuffer.Get(), 0, _depthStencilView.GetAddressOf());
}

void DepthStencilView::InitializeShadowMap(ComPtr<ID3D11Device> device, int width, int height, bool enable4xMsaa)
{
	_isShadow = true;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;								// �Ӹ� ������ ����. ���� ���ٽ� ���۸� ���� �ؽ��Ĵ� �ϳ��� ������ ��
	depthBufferDesc.ArraySize = 1;								// �ؽ�ó �迭�� �ؽ�ó ����. �굵 �ϳ��� �ʿ�
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;		// �ٸ���
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
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;	// �ٸ���
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, nullptr, _depthStencilBuffer.GetAddressOf());

	// D3D�� depth buffer�� depth stencil texture�� ����Ѵٴ� ����� �˵��� ���ٽ� description�� �ۼ��Ѵ�.
	// ����/���ٽǺ� desc �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// depth stencil view description �ۼ� 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (_enable4xMsaa)
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	else
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// ����/���ٽǺ� ����
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
