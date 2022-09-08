#pragma once

namespace GraphicsEngine
{
	class DepthStencilState
	{
	private:
		ComPtr<ID3D11DepthStencilState> _depthStencilState;

	public:
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return _depthStencilState; }

		void Initialize(ComPtr<ID3D11Device> device, bool depthEnable, D3D11_COMPARISON_FUNC depthFuc);

		void Release();
	};
}
