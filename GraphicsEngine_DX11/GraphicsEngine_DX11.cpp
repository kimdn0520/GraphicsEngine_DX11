#include "pch.h"
#include "GraphicsEngine_DX11.h"
#include "Device.h"
#include "SwapChain.h"
#include "DepthStencilState.h"

namespace GraphicsEngine
{
	void GraphicsEngine::GraphicsEngine_DX11::Initialize(HWND hwnd, int screenWidth, int screenHeight)
	{
		_windowInfo.screenWidth = screenWidth;
		_windowInfo.screenWidth = screenHeight;
		_windowInfo.hwnd = hwnd;
		_windowInfo.enable4xMSAA = true;
			
		_device = new Device();
		_swapChain = new SwapChain();
		_depthStencilState = new DepthStencilState();			// Z on
		_disableDepthStencilState = new DepthStencilState();	// Z off
		_skyBoxDepthStencilState = new DepthStencilState();		// skybox

		

		_device->Initialize(_windowInfo);
		_swapChain->Initialize(_windowInfo, _device);
	}

	void GraphicsEngine::GraphicsEngine_DX11::Release()
	{
		_device->Release();

		_swapChain->Release();
	}

	void GraphicsEngine::GraphicsEngine_DX11::OnResize(const int& screenWidth, const int& screenHeight)
	{
	}
}
