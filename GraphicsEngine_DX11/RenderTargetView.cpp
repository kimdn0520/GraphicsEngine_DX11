#include "pch.h"
#include "RenderTargetView.h"

void RenderTargetView::BackBufferRenderTarget(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const ComPtr<IDXGISwapChain>& swapChain,
	unsigned int MsaaQuality, bool enable4xMsaa, int width, int height)
{
	// Resize the swap chain and recreate the render target view.
	HRESULT hr = swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if (FAILED(hr))
		MessageBox(0, TEXT("ResizeBuffers Failed."), 0, 0);

	ComPtr<ID3D11Texture2D> backBuffer;

	// GetBuffer �޼��带 ȣ���ؼ� SwapChain�� ����Ű�� �����͸� ��´�. ù �Ű������� �ĸ� ������ �ε����̴�.
	// �ĸ���۰� �ϳ��̹Ƿ� 0�� �����Ѵ�. �ι�°�� ������ �������̽� ������ ���� ������� �ĸ� ���۸� ����Ű�� �����͸� �����ش�.
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	// �ĸ���ۿ� ���� ���� Ÿ�ٺ並 ����
	device->CreateRenderTargetView(backBuffer.Get(), 0, _renderTargetView.GetAddressOf());
	backBuffer.Reset();			// GetBuffer�� ȣ���ϸ� �ĸ� ���ۿ����� COM ����Ƚ���� �����ϱ� ������ �� ����ϰ� ���� Reset ���ش�.
}

bool RenderTargetView::RenderTargetTextureInit(ComPtr<ID3D11Device> device, int width, int height, DXGI_FORMAT format)
{
	_width = width;
	_height = height;

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// RTT ��ũ������ �ʱ�ȭ�մϴ�. 
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT ��ũ������ �����մϴ�. 
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// RTT�� �����մϴ�.
	result = device->CreateTexture2D(&textureDesc, NULL, _renderTarget.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// ���� Ÿ�� �信 ���� ��ũ������ �����մϴ�.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// ���� Ÿ�� �並 �����մϴ�. 
	result = device->CreateRenderTargetView(_renderTarget.Get(), &renderTargetViewDesc, _renderTargetView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// ���̴� ���ҽ� �信 ���� ��ũ������ �����մϴ�.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// ���̴� ���ҽ� �並 �����մϴ�. 
	result = device->CreateShaderResourceView(_renderTarget.Get(), &shaderResourceViewDesc, _shaderResourceView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void RenderTargetView::ClearRenderTarget(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 inputColor)
{
	float color[4];

	// Setup the color to clear the buffer to. 
	color[0] = inputColor.x;
	color[1] = inputColor.y;
	color[2] = inputColor.z;
	color[3] = inputColor.w;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
}

void RenderTargetView::Release()
{
	_renderTarget.ReleaseAndGetAddressOf();
	_renderTargetView.ReleaseAndGetAddressOf();
	_shaderResourceView.ReleaseAndGetAddressOf();
}

void RenderTargetView::ResizeRelease()
{
	_renderTargetView.ReleaseAndGetAddressOf();
}

