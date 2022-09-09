#pragma once
#include "ShaderBase.h"

namespace GraphicsEngine
{
	class PixelShader : public ShaderBase
	{
	public:
		PixelShader(const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro);
		~PixelShader();

	private:
		ComPtr<ID3D11PixelShader> _ps;

	public:
		void CreateShader(const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro) override;

		void Update() override;

		void Release() override;
	};
}


