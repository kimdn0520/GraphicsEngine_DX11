#include "pch.h"
#include "SwapChain.h"
#include "Device.h"

void SwapChain::Initialize(WindowInfo& windowInfo, std::shared_ptr<Device> _device)
{
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = windowInfo.screenWidth;
	swapChainDesc.BufferDesc.Height = windowInfo.screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA을 사용하는가? 
	if (windowInfo.enable4xMSAA)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = windowInfo.MSAAQuality_4x - 1;
	}
	// No MSAA
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = windowInfo.hwnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set to full screen or windowed mode. 
	if (windowInfo.fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// IDXGIFactory를 얻어오기 위한 과정쓰,,
	// D3디바이스로부터 dxgi디바이스를 얻어온다.
	IDXGIDevice* dxgiDevice = 0;
	_device->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	// dxgi디바이스로부터 dxgi어댑터를 얻어온다.
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	// dxgi어댑터로부터 dxgi팩토리를 얻어온다.
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	// dxgiFactory 인스턴스를 통해서 CreateSwapChain 함수를 호출하여 스왑체인을 생성한다.
	dxgiFactory->CreateSwapChain(_device->GetDevice().Get(), &swapChainDesc, _swapChain.GetAddressOf());

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
}

void SwapChain::Release()
{
	_swapChain.ReleaseAndGetAddressOf();
}

