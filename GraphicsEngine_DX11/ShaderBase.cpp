#include "pch.h"
#include "ShaderBase.h"
#include "SamplerStateDefine.h"
#include "ResourceManager.h"
#include "Texture.h"

ShaderBase::ShaderBase(SHADER_TYPE shaderType, std::wstring shaderName)
	: _shaderType(shaderType), _shaderName(shaderName)
{}

ShaderBase::~ShaderBase()
{}

void ShaderBase::AddShaderResourceViewData(std::string name, int registerSlot)
{
	_shaderResourceViewData[name] = std::make_shared<ShaderResourceBuffer>(name, registerSlot);
	
	_shaderResourceViews.resize(_shaderResourceViewData.size());
}

void ShaderBase::ReleaseData()
{
	for (auto it : _constantBufferData)
		it.second.reset();

	for (auto it : _samplerStateData)
		it.second.reset();

	for (auto it : _shaderResourceViewData)
		it.second.reset();

	_constantBufferData.clear();
	_samplerStateData.clear();
	_shaderResourceViewData.clear();

	for (auto it : _constantBuffers)
		it.Reset();

	for (auto it : _samplerStates)
		it.Reset();

	for (auto it : _shaderResourceViews)
		it.Reset();

	_constantBuffers.clear();
	_samplerStates.clear();
	_shaderResourceViews.clear();
}

void ShaderBase::SetConstantBuffer()
{
	UINT cBufferFlag = 0;

	for (auto& cBuffer : _constantBufferData)
	{
		// Flag 설정
		switch (cBuffer.second->usage)
		{
		case D3D11_USAGE_DEFAULT:
			break;
		case D3D11_USAGE_IMMUTABLE:
			break;
		case D3D11_USAGE_DYNAMIC:
			cBufferFlag = D3D11_CPU_ACCESS_WRITE;
			break;
		case D3D11_USAGE_STAGING:
			cBufferFlag = D3D11_CPU_ACCESS_READ;
			break;
		default:
			break;
		}

		ComPtr<ID3D11Buffer> buffer = nullptr;

		// description 작성
		CD3D11_BUFFER_DESC cBufferDesc(
			(UINT)cBuffer.second->size,
			D3D11_BIND_CONSTANT_BUFFER,
			cBuffer.second->usage, cBufferFlag);

		// 버퍼 생성
		Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
			CreateBuffer(&cBufferDesc, nullptr, &buffer);

		// 생성한 버퍼 넣어주기
		_constantBuffers[cBuffer.second->register_slot] = buffer;
	}
}

void ShaderBase::SetSamplerBuffer(int sampler, int register_slot)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;

	switch (sampler)
	{
	case (int)SAMSTATE::AnisotropicWrap:
	{
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MaxAnisotropy = 4;
	}
	break;
	case (int)SAMSTATE::AnisotropicClamp:
	{
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MaxAnisotropy = 4;
	}
	break;
	case (int)SAMSTATE::LinearWrap:
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	break;
	case (int)SAMSTATE::LinearClamp:
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	break;
	case (int)SAMSTATE::LinearPointBoarder:
	{
		samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		samplerDesc.BorderColor[0] = 1;
		samplerDesc.BorderColor[1] = 1;
		samplerDesc.BorderColor[2] = 1;
		samplerDesc.BorderColor[3] = 1;
	}
	break;
	}

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> sampleState;

	// sampler 생성
	Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
		CreateSamplerState(&samplerDesc, &sampleState);

	// 생성한 sampler 넣어주기
	_samplerStates[register_slot] = sampleState;
}

void ShaderBase::SetResourceViewBuffer(std::wstring texName, std::string texture)
{
	int register_slot = _shaderResourceViewData[texture]->register_slot;

	_shaderResourceViews[register_slot] = ResourceManager::Get()->GetTexture(texName)->GetMapSRV();
}

void ShaderBase::SetResourceViewBuffer(ComPtr<ID3D11ShaderResourceView> srv, std::string texture)
{
	int register_slot = _shaderResourceViewData[texture]->register_slot;

	_shaderResourceViews[register_slot] = srv;
}
