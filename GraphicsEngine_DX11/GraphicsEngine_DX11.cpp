#include "pch.h"
#include "GraphicsEngine_DX11.h"
#include "Device.h"
#include "SwapChain.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "RenderTargetView.h"
#include "BlendState.h"

#include "LightManager.h"
#include "ResourceManager.h"

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
		_solidRasterizerState->Initialize(_device->GetDevice(), D3D11_CULL_BACK, D3D11_FILL_SOLID);
		_wireRasterizerState->Initialize(_device->GetDevice(), D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
		_solidNoneCullRasterizerState->Initialize(_device->GetDevice(), D3D11_CULL_NONE, D3D11_FILL_SOLID);
		
		OnResize(screenWidth, screenHeight);

		// Manager initialize
		LightManager::Get()->Initialize();
		ResourceManager::Get()->Initialize();
	}

	void GraphicsEngine_DX11::Release()
	{
		LightManager::Get()->ResetLightInfo();
		ResourceManager::Get()->Release();

		_swapChain->Release();
		_depthStencilState->Release();
		_disableDepthStencilState->Release();
		_skyBoxDepthStencilState->Release();
		_wireRasterizerState->Release();
		_solidRasterizerState->Release();
		_solidNoneCullRasterizerState->Release();
		_alphaBlendState->Release();
		_device->Release();
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

	void GraphicsEngine_DX11::MainBackBufferRender()
	{
		float color[4];

		// Setup the color to clear the buffer to. 
		color[0] = 0.5f;
		color[1] = 0.5f;
		color[2] = 0.5f;
		color[3] = 0.5f;

		// Clear the back buffer.
		_device->GetDeviceContext()->ClearRenderTargetView(_mainRenderTargetView->GetRenderTargetView().Get(), color);
		
		// Clear the depth buffer. 
		_device->GetDeviceContext()->ClearDepthStencilView(_mainRenderTargetView->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void GraphicsEngine_DX11::RenderingDataRender()
	{
		// 렌더 전에 라이트부터 업데이트 시키자
		LightManager::Get()->Update();

		//RenderManager::Get()->Render();
	}

	void GraphicsEngine_DX11::Present()
	{
		_swapChain->GetSwapChain()->Present(0, 0);
	}

	ComPtr<ID3D11Device> GraphicsEngine_DX11::GetDevice()
	{
		return _device->GetDevice();
	}

	ComPtr<ID3D11DeviceContext> GraphicsEngine_DX11::GetDeviceContext()
	{
		return _device->GetDeviceContext();
	}

	ComPtr<ID3D11RasterizerState> GraphicsEngine_DX11::GetWire()
	{
		return _wireRasterizerState->GetrasterizerState();
	}

	ComPtr<ID3D11RasterizerState> GraphicsEngine_DX11::GetSolid()
	{
		return _solidRasterizerState->GetrasterizerState();
	}

	ComPtr<ID3D11RasterizerState> GraphicsEngine_DX11::GetSolidNoneCull()
	{
		return _solidNoneCullRasterizerState->GetrasterizerState();
	}
}
