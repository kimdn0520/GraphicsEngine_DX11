#pragma once

class Device;

class RasterizerState
{
private:
	ComPtr<ID3D11RasterizerState> _rasterizerState;

public:
	ComPtr<ID3D11RasterizerState> GetrasterizerState() { return _rasterizerState; }

	void Initialize(std::shared_ptr<Device> device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, int depthBias = 0, float slopeScaledDepthBias = 0.0f);

	void Release();
};


