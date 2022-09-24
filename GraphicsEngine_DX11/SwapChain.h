#pragma once
#include "WindowInfo.h"

class Device;

class SwapChain
{
private:
	ComPtr<IDXGISwapChain> _swapChain;

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }

	void Initialize(WindowInfo& windowInfo, std::shared_ptr<Device> _device);

	void Release();
};


