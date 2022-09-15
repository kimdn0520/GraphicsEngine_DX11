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
	// Ÿ��, ���̴� �����̸�, ���, entrypoint, ���̴� �Ŵ����� ����� �̸�, ���̴� ��ũ��
	D3D_SHADER_MACRO model_MacroVS_1[] = { {"Skinned"}, {NULL, NULL} };										 // ��Ų�� 
	CreateShader(SHADER_TYPE::VERTEX, L"Model_VS.hlsl", _filepath, "Model_VS", L"Model_VS", nullptr);
	CreateShader(SHADER_TYPE::VERTEX, L"Model_VS.hlsl", _filepath, "Model_VS", L"Model_Skinned_VS", model_MacroVS_1);
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PS.hlsl", _filepath, "Model_PS", L"Model_PS", nullptr);

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

	_samplerList.insert(std::make_pair("samAnisotropicWrap", (int)SAMSTATE::AnisotropicWrap));
	_samplerList.insert(std::make_pair("samAnisotropicClamp", (int)SAMSTATE::AnisotropicClamp));
	_samplerList.insert(std::make_pair("samLinearWrap", (int)SAMSTATE::LinearWrap));
	_samplerList.insert(std::make_pair("samLinearClamp", (int)SAMSTATE::LinearClamp));
}

void ShaderManager::AddConstantBuffer()
{
	// ���̴��� �ִ� cbuffer ���� usage ���� �� ���� ����
	for (auto& shader : _shaderList)
	{
		for (auto& cBuffer : shader.second->GetConstantBufferData())
		{
			auto it = _cbufferUsageList.find(cBuffer.first);

			// ���� Ű�� ã�Ƽ� usage�� ���´�.
			if (it != _cbufferUsageList.end())
			{
				cBuffer.second->usage = it->second;				// usage �־��ش�
			}
		}

		// usage �� �־������� ���� �������ش�.
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

	// �����ϸ鼭 ���̴�����Ʈ�� ��Ƶд�.
	_shaderList.insert(make_pair(saveName, newShader));

	return newShader;
}

ShaderBase* ShaderManager::GetShader(std::wstring name)
{
	return _shaderList[name];
}
