#pragma once

namespace GraphicsEngine
{
	class RasterizerState
	{
	private:
		ComPtr<ID3D11RasterizerState> _rasterizerState;

	public:
		ComPtr<ID3D11RasterizerState> GetrasterizerState() { return _rasterizerState; }

		void Initialize(ComPtr<ID3D11Device> device);

		void Release();
	};
}

