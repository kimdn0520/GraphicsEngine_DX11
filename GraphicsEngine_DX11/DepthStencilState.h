#pragma once

class Device;

class DepthStencilState
{
private:
	ComPtr<ID3D11DepthStencilState> _depthStencilState;

public:
	ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return _depthStencilState; }

	void Initialize(std::shared_ptr<Device> device, bool depthEnable, D3D11_COMPARISON_FUNC depthFuc);

	void Release();
};

