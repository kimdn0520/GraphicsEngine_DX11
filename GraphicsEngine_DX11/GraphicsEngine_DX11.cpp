#include "pch.h"
#include "GraphicsEngine_DX11.h"
#include "Device.h"
#include "SwapChain.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "BlendState.h"
#include "ViewPort.h"

#include "LightManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "RenderManager.h"

void GraphicsEngine_DX11::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	_windowInfo.screenWidth = screenWidth;
	_windowInfo.screenWidth = screenHeight;
	_windowInfo.hwnd = hwnd;
	_windowInfo.enable4xMSAA = true;
			
	_device = new Device();
	_swapChain = new SwapChain();
	_mainRenderTargetView = new RenderTargetView();
	_depthStencilView = new DepthStencilView();
	_depthStencilState = new DepthStencilState();			// Z on
	_disableDepthStencilState = new DepthStencilState();	// Z off
	_skyBoxDepthStencilState = new DepthStencilState();		// skybox
	_wireRasterizerState = new RasterizerState();			// wire
	_solidRasterizerState = new RasterizerState();			// solid
	_solidNoneCullRasterizerState = new RasterizerState();	// solidNoneCull
	_alphaBlendState = new BlendState();
	_mainViewPort = new ViewPort();

	_device->Initialize(_windowInfo);
	_swapChain->Initialize(_windowInfo, _device);
	_solidRasterizerState->Initialize(_device, D3D11_CULL_BACK, D3D11_FILL_SOLID);
	_wireRasterizerState->Initialize(_device, D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
	_solidNoneCullRasterizerState->Initialize(_device, D3D11_CULL_NONE, D3D11_FILL_SOLID);
	_mainViewPort->Initialize(Vector2::Zero, screenWidth, screenHeight);
	_depthStencilView->Initialize(_device->GetDevice(), screenWidth, screenHeight, true);

	OnResize(screenWidth, screenHeight);

	// Manager initialize
	LightManager::Get()->Initialize();
	ResourceManager::Get()->Initialize();
	ShaderManager::Get()->SetFilePath(L"Data/Shader/");
	ShaderManager::Get()->CreateAllShaders();
	RenderManager::Get()->Initialize();
}

void GraphicsEngine_DX11::Release()
{
	LightManager::Get()->ResetLightInfo();
	ResourceManager::Get()->Release();
	ShaderManager::Get()->Release();

	_swapChain->Release();
	_depthStencilState->Release();
	_disableDepthStencilState->Release();
	_skyBoxDepthStencilState->Release();
	_wireRasterizerState->Release();
	_solidRasterizerState->Release();
	_solidNoneCullRasterizerState->Release();
	_alphaBlendState->Release();
	_device->Release();
	_mainViewPort->Release();
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

		_depthStencilState->Initialize(_device, true, D3D11_COMPARISON_LESS);
		_disableDepthStencilState->Initialize(_device, false, D3D11_COMPARISON_LESS);
		_skyBoxDepthStencilState->Initialize(_device, false, D3D11_COMPARISON_LESS_EQUAL);
		_alphaBlendState->Initialize(_device);
		_mainViewPort->OnResize(screenWidth, screenHeight);
		_depthStencilView->OnResize(_device->GetDevice(), screenWidth, screenHeight);

		// RSSetViewports
		_mainViewPort->SetViewPort(_device->GetDeviceContext());

		// OnResize 시 주의 
		_device->GetDeviceContext()->OMSetDepthStencilState(_depthStencilState->GetDepthStencilState().Get(), 1);

		if(RenderManager::Get()->isInit)
			RenderManager::Get()->OnResize(screenWidth, screenHeight);
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
	_device->GetDeviceContext()->ClearDepthStencilView(_depthStencilView->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsEngine_DX11::RenderingDataRender()
{
	// 렌더 전에 라이트부터 업데이트 시키자
	LightManager::Get()->Update();

	RenderManager::Get()->Render();
}

void GraphicsEngine_DX11::Present()
{
	_swapChain->GetSwapChain()->Present(0, 0);
}

int GraphicsEngine_DX11::GetScreenWidth()
{
	return _windowInfo.screenWidth;
}

int GraphicsEngine_DX11::GetScreenHeight()
{
	return _windowInfo.screenHeight;
}

void GraphicsEngine_DX11::RenderToBackBuffer()
{
	MainBackBufferRender();

	_device->GetDeviceContext()->OMSetRenderTargets(1, _mainRenderTargetView->GetRenderTargetView().GetAddressOf(), _depthStencilView->GetDepthStencilView().Get());

	_mainViewPort->SetViewPort(_device->GetDeviceContext());
}

void GraphicsEngine_DX11::TurnZBufferOn()
{
	_device->GetDeviceContext()->OMSetDepthStencilState(_depthStencilState->GetDepthStencilState().Get(), 1);
}

void GraphicsEngine_DX11::TurnZBufferOff()
{
	_device->GetDeviceContext()->OMSetDepthStencilState(_disableDepthStencilState->GetDepthStencilState().Get(), 1);
}

void GraphicsEngine_DX11::TurnSkyboxOn()
{
	_device->GetDeviceContext()->OMSetDepthStencilState(_skyBoxDepthStencilState->GetDepthStencilState().Get(), 1);
}

Device* GraphicsEngine_DX11::GetDeviceClass()
{
	return _device;
}

RasterizerState* GraphicsEngine_DX11::GetWire()
{
	return _wireRasterizerState;
}

RasterizerState* GraphicsEngine_DX11::GetSolid()
{
	return _solidRasterizerState;
}

RasterizerState* GraphicsEngine_DX11::GetSolidNoneCull()
{
	return _solidNoneCullRasterizerState;
}

void GraphicsEngine_DX11::SendObjectRenderingData(ObjectInfo* objectInfo)
{
	RenderManager::Get()->PushRenderData(objectInfo);
}

void GraphicsEngine_DX11::SendCameraData(CameraInfo* cameraInfo)
{
	RenderManager::s_cameraInfo = cameraInfo;
}

void GraphicsEngine_DX11::SendLightData(LightInfo* lightInfo)
{
	LightManager::Get()->SetLightInfo(lightInfo);
}

void GraphicsEngine_DX11::UpdateLightData(LightInfo* lightInfo)
{
	LightManager::Get()->UpdateLightInfo(lightInfo);
}

size_t GraphicsEngine_DX11::CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState)
{
	return ResourceManager::Get()->CreateMesh(vertices, indicies, topology, rasterState);
}

size_t GraphicsEngine_DX11::CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState)
{
	return ResourceManager::Get()->CreateMesh(vertices, indicies, topology, rasterState);
}

