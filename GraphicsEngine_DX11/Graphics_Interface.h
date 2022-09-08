#pragma once
#include "GraphicsEngineDLL.h"

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
};



