#pragma once
#include "ShaderBase.h"

class ShaderManager
{
private:
	static ShaderManager* shaderManager;

public:
	static ShaderManager* Get();

private:
	std::unordered_map<std::wstring, ShaderBase*> _shaderList;

	// cbuffer�� ����ü������ usage�� SetUsage�Լ����� �����ؼ� �־�����
	std::unordered_map<std::string, D3D11_USAGE> _cbufferUsageList;

	std::map<std::string, int> _samplerList;

	std::wstring _filepath;

public:
	// ���ʷ� �ؾ��� ��.. filePath ���̴��� ����ִ� ���͸� ��� ����
	void SetFilePath(const std::wstring& filePath) { _filepath = filePath; }

	// �� ���� ��� ���̴��� ������ش�.
	void CreateAllShaders();

	void Release();

	void SetInfo();

	void AddConstantBuffer();

	void AddSamplerBuffer();

	ShaderBase* CreateShader(SHADER_TYPE shaderType, const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const std::wstring& saveName, const D3D_SHADER_MACRO* macro);

	ShaderBase* GetShader(std::wstring name);
};


