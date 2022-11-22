#include "pch.h"
#include "ShaderManager.h"
#include "ConstantBufferDefine.h"
#include "SamplerStateDefine.h"
#include "ShaderResource.h"
#include "VertexShader.h"
#include "PixelShader.h"

std::shared_ptr<ShaderManager> ShaderManager::shaderManager = nullptr;

std::shared_ptr<ShaderManager> ShaderManager::Get()
{
	if (shaderManager == nullptr)
		shaderManager = std::make_shared<ShaderManager>();

	return shaderManager;
}

void ShaderManager::CreateAllShaders()
{
	// Ÿ��, ���̴� �����̸�, ���, entrypoint, ���̴� �Ŵ����� ����� �̸�, ���̴� ��ũ��
	CreateShader(SHADER_TYPE::VERTEX, L"Quad_VS.hlsl", _filepath, "Quad_VS", L"Quad_VS", nullptr);
	
	CreateShader(SHADER_TYPE::PIXEL, L"Debug_PS.hlsl", _filepath, "Debug_PS", L"Debug_PS", nullptr);

	D3D_SHADER_MACRO model_MacroVS_1[] = { {"Skinned"}, {NULL, NULL} };										  
	//CreateShader(SHADER_TYPE::VERTEX, L"Model_VS.hlsl", _filepath, "Model_VS", L"Model_VS", nullptr);
	//CreateShader(SHADER_TYPE::VERTEX, L"Model_VS.hlsl", _filepath, "Model_VS", L"Model_Skinned_VS", model_MacroVS_1);
	//CreateShader(SHADER_TYPE::PIXEL, L"Model_PS.hlsl", _filepath, "Model_PS", L"Model_PS", nullptr);

	CreateShader(SHADER_TYPE::VERTEX, L"Model_PBR_VS.hlsl", _filepath, "Model_PBR_VS", L"Model_PBR_VS", nullptr);
	CreateShader(SHADER_TYPE::VERTEX, L"Model_PBR_VS.hlsl", _filepath, "Model_PBR_VS", L"Model_PBR_Skinned_VS", model_MacroVS_1);

	D3D_SHADER_MACRO model_MacroPS_1[] = { {"USE_ALBEDO"}, {NULL, NULL} };
	D3D_SHADER_MACRO model_MacroPS_2[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {NULL, NULL} };
	D3D_SHADER_MACRO model_MacroPS_3[] = { {"USE_ALBEDO"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {NULL, NULL} };
	D3D_SHADER_MACRO model_MacroPS_4[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {NULL, NULL} };
	D3D_SHADER_MACRO model_MacroPS_5[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_AO"}, {NULL, NULL} };
	D3D_SHADER_MACRO model_MacroPS_6[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {"USE_AO"}, {NULL, NULL} };
	D3D_SHADER_MACRO model_MacroPS_7[] = { {"USE_ALBEDO"}, {"USE_NORMAL"}, {"USE_METALLIC"}, {"USE_ROUGHNESS"}, {"USE_AO"}, {"USE_EMISSIVE"}, {NULL, NULL} };

	// basic
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_PS", nullptr);
	
	// albedo
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_C_PS", model_MacroPS_1);
	
	// albedo, normal
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_CN_PS", model_MacroPS_2);
	
	// albedo, metallic, roughness
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_CMR_PS", model_MacroPS_3);
	
	// albedo, normal, metallic, roughness
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_CNMR_PS", model_MacroPS_4);
	
	// albedo, normal, ao
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_CNA_PS", model_MacroPS_5);
	
	// albedo, normal, metallic, roughness, ao
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_CNMRA_PS", model_MacroPS_6);
	
	// albedo, normal, metallic, roughness, ao, emissive
	CreateShader(SHADER_TYPE::PIXEL, L"Model_PBR_PS.hlsl", _filepath, "Model_PBR_PS", L"Model_PBR_CNMRAE_PS", model_MacroPS_7);


	D3D_SHADER_MACRO shadow_MacroVS_1[] = { {"Skinned"}, {NULL, NULL} };										
	CreateShader(SHADER_TYPE::VERTEX, L"Shadow_VS.hlsl", _filepath, "Shadow_VS", L"Shadow_VS", nullptr);
	CreateShader(SHADER_TYPE::VERTEX, L"Shadow_VS.hlsl", _filepath, "Shadow_VS", L"Shadow_Skinned_VS", shadow_MacroVS_1);

	D3D_SHADER_MACRO light_MacroPS_1[] = { {"SHADOW_MACRO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_MacroPS_2[] = { {"SSAO_MACRO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light_MacroPS_3[] = { {"SHADOW_MACRO", "SSAO_MACRO"}, {NULL, NULL} };

	CreateShader(SHADER_TYPE::PIXEL, L"Light_PS.hlsl", _filepath, "Light_PS", L"Light_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Light_PBR_PS.hlsl", _filepath, "Light_PBR_PS", L"Light_PBR_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Light_PBR_PS.hlsl", _filepath, "Light_PBR_PS", L"Light_PBR_PS1", light_MacroPS_1);
	CreateShader(SHADER_TYPE::PIXEL, L"Light_PBR_PS.hlsl", _filepath, "Light_PBR_PS", L"Light_PBR_PS2", light_MacroPS_2);
	CreateShader(SHADER_TYPE::PIXEL, L"Light_PBR_PS.hlsl", _filepath, "Light_PBR_PS", L"Light_PBR_PS3", light_MacroPS_3);

	CreateShader(SHADER_TYPE::PIXEL, L"ToneMapping_PS.hlsl", _filepath, "ToneMapping_PS", L"ToneMapping_PS", nullptr);

	CreateShader(SHADER_TYPE::VERTEX, L"Skybox_VS.hlsl", _filepath, "Skybox_VS", L"Skybox_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Skybox_PS.hlsl", _filepath, "Skybox_PS", L"Skybox_PS", nullptr);

	CreateShader(SHADER_TYPE::PIXEL, L"GaussianBlur_PS.hlsl", _filepath, "BlurX_PS", L"BlurX_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"GaussianBlur_PS.hlsl", _filepath, "BlurY_PS", L"BlurY_PS", nullptr);

	SetInfo();
	AddConstantBuffer();
	AddSamplerBuffer();
}

void ShaderManager::Release()
{
	for (auto& shader : _shaderList)
	{
		shader.second->ReleaseData();
		//shader.second->Release();
	}

	_shaderList.clear();
	_cbufferUsageList.clear();
	_samplerList.clear();
}

void ShaderManager::SetInfo()
{
	_cbufferUsageList.insert(std::make_pair("cbPerObject", D3D11_USAGE_DEFAULT));
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

std::shared_ptr<ShaderBase> ShaderManager::CreateShader(SHADER_TYPE shaderType, const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const std::wstring& saveName, const D3D_SHADER_MACRO* macro)
{
	std::shared_ptr<ShaderBase> newShader = nullptr;

	switch (shaderType)
	{
	case SHADER_TYPE::VERTEX:
		newShader = std::make_shared<VertexShader>(shaderName, path, entryPoint, macro);
		break;
	case SHADER_TYPE::PIXEL:
		newShader = std::make_shared<PixelShader>(shaderName, path, entryPoint, macro);
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

std::shared_ptr<ShaderBase> ShaderManager::GetShader(std::wstring name)
{
	return _shaderList[name];
}
