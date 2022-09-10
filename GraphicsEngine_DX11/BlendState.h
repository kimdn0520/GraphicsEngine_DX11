#pragma once

class Device;

class BlendState
{
private:
	ComPtr<ID3D11BlendState> _blendState;

public:
	void Initialize(Device* device);

	ComPtr<ID3D11BlendState> GetBlendState() { return _blendState; }

	void Release();
};


