#pragma once

class DepthStencilView
{
private:
	ComPtr<ID3D11Texture2D> _depthStencilBuffer;
	
	ComPtr<ID3D11DepthStencilView> _depthStencilView;

	// shadow..?
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

	bool _enable4xMsaa;

	bool _isShadow = false;

public:
	ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return _depthStencilView; }

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return _shaderResourceView; }

	void Initialize(ComPtr<ID3D11Device> device, int width, int height, bool enable4xMsaa = false);

	void InitializeShadowMap(ComPtr<ID3D11Device> device, int width, int height, bool enable4xMsaa = false);

	void OnResize(ComPtr<ID3D11Device> device, int width, int height);

	void Release();

	void ClearDepthStencilView(ComPtr<ID3D11DeviceContext> deviceContext);
};

