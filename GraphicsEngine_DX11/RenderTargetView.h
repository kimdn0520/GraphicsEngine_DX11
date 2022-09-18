#pragma once

class RenderTargetView
{
private:
	ComPtr<ID3D11Texture2D> _renderTarget;					    // ±◊∑¡¡Ÿ ∞¯∞£
	ComPtr<ID3D11RenderTargetView> _renderTargetView;			// ∑ª¥ı≈∏∞Ÿ ∫‰
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;		// ºŒ¿Ã¥ı ∏Æº“Ω∫ ∫‰

public:
	void BackBufferRenderTarget(
		const ComPtr<ID3D11Device>& device,
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const ComPtr<IDXGISwapChain>& swapChain,
		unsigned int MsaaQuality,
		bool enable4xMsaa,
		int width, int height);

	bool RenderTargetTextureInit(ComPtr<ID3D11Device> device, int width, int height, DXGI_FORMAT format);

	ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return _renderTargetView; }
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _shaderResourceView; }

	void ClearRenderTarget(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 inputColor);

	void Release();
	void ResizeRelease();
};



