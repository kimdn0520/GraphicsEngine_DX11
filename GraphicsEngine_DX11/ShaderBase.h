#pragma once
#include "Graphics_Interface.h"
#include "ShaderResource.h"
#include "Device.h"
#include <string>

enum class SHADER_TYPE
{
	VERTEX,
	PIXEL,
	COMPUTE,
};

class ShaderBase
{
public:
	ShaderBase(SHADER_TYPE shaderType, std::wstring shaderName);
	~ShaderBase();

private:
	SHADER_TYPE _shaderType;
	std::wstring _shaderName;

protected:
	std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> _constantBufferData;
	std::unordered_map<std::string, std::shared_ptr<SamplerBuffer>> _samplerStateData;
	std::unordered_map<std::string, std::shared_ptr<ShaderResourceBuffer>> _shaderResourceViewData;

	std::vector<ComPtr<ID3D11Buffer>> _constantBuffers;
	std::vector<ComPtr<ID3D11SamplerState>> _samplerStates;
	std::vector<ComPtr<ID3D11ShaderResourceView>> _shaderResourceViews;

public:
	std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>& GetConstantBufferData() { return _constantBufferData; }
	std::unordered_map<std::string, std::shared_ptr<SamplerBuffer>>& GetSamplerStateData() { return _samplerStateData; }
	std::unordered_map<std::string, std::shared_ptr<ShaderResourceBuffer>>& GetShaderResourceViewData() { return _shaderResourceViewData; }

	void AddShaderResourceViewData(std::string name, int registerSlot);

	void ReleaseData();

	void SetConstantBuffer();
	void SetSamplerBuffer(int sampler, int register_slot);

	// 오버로딩
	void SetResourceViewBuffer(std::wstring texName, std::string texture);
	void SetResourceViewBuffer(ComPtr<ID3D11ShaderResourceView> srv, std::string texture);

public:
	template <typename T>
	void ConstantBufferUpdate(T* cbuffer, std::string name);

	virtual void CreateShader(const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro) abstract;

	virtual void Release() abstract;

	virtual void Update() abstract;
};

template<typename T>
inline void ShaderBase::ConstantBufferUpdate(T* cbuffer, std::string name)
{
	auto it = _constantBufferData.find(name);

	switch (it->second->usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()
			->UpdateSubresource(_constantBuffers[it->second->register_slot].Get(),
				0, nullptr,
				cbuffer, 0, 0);
	}
	break;
	case D3D11_USAGE_DYNAMIC:
	{
		// Mapping SubResource Data..
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// GPU Access Lock Buffer Data..
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()
			->Map(_constantBuffers[it->second->register_slot].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		// Copy Resource Data..
		memcpy(mappedResource.pData, cbuffer, it->second->size);

		// GPU Access UnLock Buffer Data..
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()
			->Unmap(_constantBuffers[it->second->register_slot].Get(), 0);
	}
	break;
	case D3D11_USAGE_STAGING:
		break;
	default:
		break;
	}
}



