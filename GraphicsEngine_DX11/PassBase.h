#pragma once
#include "Graphics_RenderingData.h"

class Graphics_Interface;

class PassBase
{
public:
	PassBase() = default;
	virtual ~PassBase() = default;

public:
	static void Initialize();

	static void Reset();

protected:
	static ComPtr<ID3D11Device> g_device;

	static ComPtr<ID3D11DeviceContext> g_deviceContext;

public:
	virtual void Start() {};

	virtual void Release() abstract;

	virtual void OnResize(int width, int height) {};
};

