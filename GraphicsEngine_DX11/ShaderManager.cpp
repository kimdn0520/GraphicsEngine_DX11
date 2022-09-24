#include "pch.h"
#include "ShaderManager.h"
#include "ConstantBufferDefine.h"
#include "SamplerStateDefine.h"
#include "ShaderResource.h"
#include "VertexShader.h"
#include "PixelShader.h"

ShaderManager* ShaderManager::shaderManager = nullptr;

ShaderManager* ShaderManager::Get()
{
	if (shaderManager == nullptr)
		shaderManager = new ShaderManager();

	return shaderManager;
}

void ShaderManager::CreateAllShaders()
{
	// 타입, 셰이더 파일이름, 경로, entrypoint, 셰이더 매니저에 저장될 이름, 셰이더 매크로
	CreateShader(SHADER_TYPE::VERTEX, L"Quad_VS.hlsl", _filepath, "Quad_VS", L"Quad_VS", nullptr);
	
	CreateShader(SHADER_TYPE::PIXEL, L"Debug_PS.hlsl", _filepath, "Debug_PS", L"Debug_PS", nullptr);

	D3D_SHADER_MACRO model_MacroVS_1[] = { {"Skinned"}, {NULL, NULL} };										  
	CreateShader(SHADER_TYPE::VERTEX, L"Model_VS.hlsl", _filepath, "Model_VS", L"Model_VS", nullptr);
	CreateShader(SHADER_TYPE::VERTEX, L"Model_VS.hlsl", _filepath, "Model_VS", L"Model_Skinned_VS", model_MacroVS_1);
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PS.hlsl", _filepath, "Model_PS", L"Model_PS", nullptr);

	D3D_SHADER_MACRO shadow_MacroVS_1[] = { {"Skinned"}, {NULL, NULL} };										
	CreateShader(SHADER_TYPE::VERTEX, L"Shadow_VS.hlsl", _filepath, "Shadow_VS", L"Shadow_VS", nullptr);
	CreateShader(SHADER_TYPE::VERTEX, L"Shadow_VS.hlsl", _filepath, "Shadow_VS", L"Shadow_Skinned_VS", shadow_MacroVS_1);

	CreateShader(SHADER_TYPE::PIXEL, L"Light_PS.hlsl", _filepath, "Light_PS", L"Light_PS", nullptr);

	CreateShader(SHADER_TYPE::PIXEL, L"ToneMapping_PS.hlsl", _filepath, "ToneMapping_PS", L"ToneMapping_PS", nullptr);

	SetInfo();
	AddConstantBuffer();
	AddSamplerBuffer();
}

void ShaderManager::Release()
{
	for (auto& shader : _shaderList)
	{
		shader.second->ReleaseData();
		shader.second->Release();
	}

	_shaderList.clear();
	_cbufferUsageList.clear();
	_samplerList.clear();
}

void ShaderManager::SetInfo()
{
	_cbufferUsageList.insert(std::make_pair("cbMesh", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(std::make_pair("cbSkinned", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(std::make_pair("cbMaterial", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(std::make_pair("cbLight", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(std::make_pair("cbTexture", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(std::make_pair("cbLightViewProj", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(std::make_pair("cbToneMapping", D3D11_USAGE_DEFAULT)); 

	_samplerList.insert(std::make_pair("samAnisotropicWrap", (int)SAMSTATE::AnisotropicWrap));
	_samplerList.insert(std::make_pair("samAnisotropicClamp", (int)SAMSTATE::AnisotropicClamp));
	_samplerList.insert(std::make_pair("samLinearWrap", (int)SAMSTATE::LinearWrap));
	_samplerList.insert(std::make_pair("samLinearClamp", (int)SAMSTATE::LinearClamp));
	_samplerList.insert(std::make_pair("samLinearPointBoarder", (int)SAMSTATE::LinearPointBoarder));
}

void ShaderManager::AddConstantBuffer()
{
	// 쉐이더에 있는 cbuffer 마다 usage 장착 및 버퍼 생성
	for (auto& shader : _shaderList)
	{
		for (auto& cBuffer : shader.second->GetConstantBufferData())
		{
			auto it = _cbufferUsageList.find(cBuffer.first);

			// 같은 키를 찾아서 usage를 얻어온다.
			if (it != _cbufferUsageList.end())
			{
				cBuffer.second->usage = it->second;				// usage 넣어준다
			}
		}

		// usage 다 넣어줬으니 버퍼 생성해준다.
		shader.second->SetConstantBuffer();
	}
}

void ShaderManager::AddSamplerBuffer()
{
	for (auto& shader : _shaderList)
	{
		for (auto& samplerState : shader.second->GetSamplerStateData())
		{
			shader.second->SetSamplerBuffer(_samplerList[samplerState.second->name], samplerState.second->register_slot);
		}
	}
}

ShaderBase* ShaderManager::CreateShader(SHADER_TYPE shaderType, const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const std::wstring& saveName, const D3D_SHADER_MACRO* macro)
{
	ShaderBase* newShader = nullptr;

	switch (shaderType)
	{
	case SHADER_TYPE::VERTEX:
		newShader = new VertexShader(shaderName, path, entryPoint, macro);
		break;
	case SHADER_TYPE::PIXEL:
		newShader = new PixelShader(shaderName, path, entryPoint, macro);
		break;
	case SHADER_TYPE::COMPUTE:
		// newShader = new ComputeShader(shaderName, path);
		break;
	default:
		return nullptr;
	}

	if (newShader == nullptr)
		return nullptr;

	// 생성하면서 쉐이더리스트에 모아둔다.
	_shaderList.insert(make_pair(saveName, newShader));

	return newShader;
}

ShaderBase* ShaderManager::GetShader(std::wstring name)
{
	return _shaderList[name];
}
