#pragma once
#include "GraphicsEngineDLL.h"

class DepthStencilState;
class BlendState;
class RenderTargetView;
class Device;
class RasterizerState;

class Graphics_Interface
{
public:
	Graphics_Interface() {};
	virtual ~Graphics_Interface() {};

private:
	static Graphics_Interface* graphicsEngine;

public:
	static GraphicsEngine_DLL Graphics_Interface* Get();

public:
	virtual GraphicsEngine_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;

	virtual GraphicsEngine_DLL void Release() abstract;

	virtual GraphicsEngine_DLL void OnResize(const int& screenWidth, const int& screenHeight) abstract;

	virtual GraphicsEngine_DLL void MainBackBufferRender() abstract;

	virtual GraphicsEngine_DLL void RenderingDataRender() abstract;

	virtual GraphicsEngine_DLL void Present() abstract;

	virtual GraphicsEngine_DLL Device* GetDeviceClass() abstract;
							   
	virtual GraphicsEngine_DLL RasterizerState* GetWire() abstract;
							   
	virtual GraphicsEngine_DLL RasterizerState* GetSolid() abstract;
							   
	virtual GraphicsEngine_DLL RasterizerState* GetSolidNoneCull() abstract;
};



