#pragma once

class Device;

class RasterizerState
{
private:
	ComPtr<ID3D11RasterizerState> _rasterizerState;

public:
	ComPtr<ID3D11RasterizerState> GetrasterizerState() { return _rasterizerState; }

	void Initialize(Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);

	void Release();
};


