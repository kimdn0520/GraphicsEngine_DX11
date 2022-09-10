#pragma once
#include "WindowInfo.h"

class Device;

class SwapChain
{
private:
	ComPtr<IDXGISwapChain> _swapChain;

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }

	void Initialize(WindowInfo& windowInfo, Device* _device);

	void Release();
};


