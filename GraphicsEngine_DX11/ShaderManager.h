#pragma once
#include "ShaderBase.h"

class ShaderManager
{
private:
	static std::shared_ptr<ShaderManager> shaderManager;

public:
	static std::shared_ptr<ShaderManager> Get();

private:
	std::unordered_map<std::wstring, std::shared_ptr<ShaderBase>> _shaderList;

	// cbuffer의 구조체에대한 usage를 SetUsage함수에서 설정해서 넣어주자
	std::unordered_map<std::string, D3D11_USAGE> _cbufferUsageList;

	std::map<std::string, int> _samplerList;

	std::wstring _filepath;

public:
	// 최초로 해야할 거.. filePath 쉐이더가 들어있는 디렉터리 경로 설정
	void SetFilePath(const std::wstring& filePath) { _filepath = filePath; }

	// 그 다음 모든 쉐이더를 만들어준다.
	void CreateAllShaders();

	void Release();

	void SetInfo();

	void AddConstantBuffer();

	void AddSamplerBuffer();

	std::shared_ptr<ShaderBase> CreateShader(SHADER_TYPE shaderType, const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const std::wstring& saveName, const D3D_SHADER_MACRO* macro);
	
	std::shared_ptr<ShaderBase> GetShader(std::wstring name);
};


