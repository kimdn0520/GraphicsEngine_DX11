#pragma once
#include "ShaderBase.h"

class VertexShader : public ShaderBase
{
public:
	VertexShader(const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro);
	~VertexShader();

private:
	ComPtr<ID3D11VertexShader> _vs;

	ComPtr<ID3D11InputLayout> _inputLayout;

public:
	void CreateShader(const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro) override;

	void Update() override;

	void Release() override;
};



