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

	// GetBuffer 메서드를 호출해서 SwapChain을 가리키는 포인터를 얻는다. 첫 매개변수는 후면 버퍼의 인덱스이다.
	// 후면버퍼가 하나이므로 0을 지정한다. 두번째는 버퍼의 인터페이스 형식을 지정 세번재는 후면 버퍼를 가리키는 포인터를 돌려준다.
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	// 후면버퍼에 대한 렌더 타겟뷰를 생성
	device->CreateRenderTargetView(backBuffer.Get(), 0, _renderTargetView.GetAddressOf());
	backBuffer.Reset();			// GetBuffer를 호출하면 후면 버퍼에대한 COM 참조횟수가 증가하기 때문에 다 사용하고 나면 Reset 해준다.
}

bool RenderTargetView::RenderTargetTextureInit(ComPtr<ID3D11Device> device, int width, int height, DXGI_FORMAT format)
{
	_width = width;
	_height = height;

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// RTT 디스크립션을 초기화합니다. 
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT 디스크립션을 세팅합니다. 
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

	// RTT를 생성합니다.
	result = device->CreateTexture2D(&textureDesc, NULL, _renderTarget.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// 렌더 타겟 뷰에 대한 디스크립션을 설정합니다.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// 렌더 타겟 뷰를 생성합니다. 
	result = device->CreateRenderTargetView(_renderTarget.Get(), &renderTargetViewDesc, _renderTargetView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// 셰이더 리소스 뷰에 대한 디스크립션을 설정합니다.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// 셰이더 리소스 뷰를 생성합니다. 
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

