#pragma once
#include "Graphics_Interface.h"
#include "WindowInfo.h"

class Device;
class SwapChain;
class DepthStencilState;
class RasterizerState;
class RenderTargetView;
class BlendState;
		
class GraphicsEngine_DX11 : public Graphics_Interface
{
public:
	GraphicsEngine_DX11() = default;
	virtual ~GraphicsEngine_DX11() = default;

private:
	WindowInfo _windowInfo;

	Device* _device;
	SwapChain* _swapChain;

	DepthStencilState* _depthStencilState;
	DepthStencilState* _disableDepthStencilState;
	DepthStencilState* _skyBoxDepthStencilState;

	RasterizerState* _wireRasterizerState;
	RasterizerState* _solidRasterizerState;
	RasterizerState* _solidNoneCullRasterizerState;

	RenderTargetView* _mainRenderTargetView;

	BlendState* _alphaBlendState;
		
public:
	virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	virtual void Release() override;

	virtual void OnResize(const int& screenWidth, const int& screenHeight) override;

	virtual void MainBackBufferRender() override;
		 
	virtual void RenderingDataRender() override;

	virtual void Present() override;

	virtual	int GetScreenWidth() override;
	
	virtual	int GetScreenHeight() override;
	
	virtual Device* GetDeviceClass() override;

	virtual RasterizerState* GetWire() override;

	virtual RasterizerState* GetSolid() override;

	virtual RasterizerState* GetSolidNoneCull() override;

};



