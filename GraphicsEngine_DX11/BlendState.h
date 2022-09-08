#pragma once

namespace GraphicsEngine
{
	class BlendState
	{
	private:
		ComPtr<ID3D11BlendState> _blendState;

	public:
		void Initialize(ComPtr<ID3D11Device> device);

		ComPtr<ID3D11BlendState> GetBlendState() { return _blendState; }

		void Release();
	};
}

