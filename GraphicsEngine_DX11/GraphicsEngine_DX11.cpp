#include "pch.h"
#include "GraphicsEngine_DX11.h"
#include "Device.h"
#include "SwapChain.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "RenderTargetView.h"
#include "BlendState.h"

namespace GraphicsEngine
{
	void GraphicsEngine_DX11::Initialize(HWND hwnd, int screenWidth, int screenHeight)
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
		_wireRasterizerState = new RasterizerState();			// wire
		_solidRasterizerState = new RasterizerState();			// solid
		_solidNoneCullRasterizerState = new RasterizerState();	// solidNoneCull
		_alphaBlendState = new BlendState();

		_device->Initialize(_windowInfo);
		_swapChain->Initialize(_windowInfo, _device);
		
		OnResize(screenWidth, screenHeight);
	}

	void GraphicsEngine_DX11::Release()
	{
		_device->Release();

		_swapChain->Release();

		_depthStencilState->Release();

		_disableDepthStencilState->Release();

		_skyBoxDepthStencilState->Release();
	}

	void GraphicsEngine_DX11::OnResize(const int& screenWidth, const int& screenHeight)
	{
		_windowInfo.screenWidth = screenWidth;
		_windowInfo.screenHeight = screenHeight;

		if (_device->GetDevice() != nullptr)
		{
			// 버퍼에 대한 참조를 보유하므로 이전 view, depth를 해제해줘야한다!! 
			_mainRenderTargetView->ResizeRelease();

			_mainRenderTargetView->BackBufferRenderTarget(
				_device->GetDevice(),
				_device->GetDeviceContext().Get(),
				_swapChain->GetSwapChain().Get(),
				_windowInfo.MSAAQuality_4x,
				_windowInfo.enable4xMSAA,
				_windowInfo.screenWidth,
				_windowInfo.screenHeight
			);

			_depthStencilState->Initialize(_device->GetDevice(), true, D3D11_COMPARISON_LESS);
			_disableDepthStencilState->Initialize(_device->GetDevice(), false, D3D11_COMPARISON_LESS);
			_skyBoxDepthStencilState->Initialize(_device->GetDevice(), false, D3D11_COMPARISON_LESS_EQUAL);
			_alphaBlendState->Initialize(_device->GetDevice());

			// OnResize 시 주의 
			_device->GetDeviceContext()->OMSetDepthStencilState(_depthStencilState->GetDepthStencilState().Get(), 1);
		}
	}
}
