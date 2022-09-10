#pragma once
#include "Graphics_Interface.h"

class GraphicsManager
{
public:
	GraphicsManager() {};

	~GraphicsManager() {};

private:
	static GraphicsManager* graphicsManager;

private:
	Graphics_Interface* graphicsEngine;
	
public:
	static GraphicsManager* Get();

public:
	void Initialize(HWND hWnd, int screenWidth, int screenHeight);

	void Release();

	void OnResize(int width, int height);

	void MainBackBufferRender();

	void RenderingDataRender();

	void Present();
};


