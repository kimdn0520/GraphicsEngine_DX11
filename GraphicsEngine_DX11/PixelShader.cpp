#include "pch.h"
#include "PixelShader.h"
#include "Graphics_Interface.h"
#include "ShaderResource.h"
#include "Device.h"

PixelShader::PixelShader(const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro)
	: ShaderBase(SHADER_TYPE::PIXEL, shaderName)
{
	CreateShader(path + shaderName, entryPoint, macro);		// 객체가 생성될때 쉐이더를 만든다.
}

PixelShader::~PixelShader()
{}

void PixelShader::CreateShader(const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro)
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	ID3D11ShaderReflection* pReflection = nullptr;

	DWORD shaderFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(path.c_str(), macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), "ps_5_0", shaderFlags, NULL, &compiledShader, &compilationMsgs);

	if (FAILED(hr))
		MessageBox(0, TEXT("pixelshader Failed."), 0, 0);

	if (compilationMsgs != 0)
		compilationMsgs->Release();

	// pixel shader 생성
	Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
		CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, _ps.GetAddressOf());

	// reflection 생성
	D3DReflect(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection);

	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);

	// Cbuffer, Sampler, Texture Data 설정
	// 음.. 해당 구조체의 name, bindpoint(register slot), size 데이터들을 저장해서 기억해두고있는다..
	int cbufferRegisterSlot = 0;
	int srvRegisterSlot = 0;
	int samplerRegisterSlot = 0;

	for (int i = 0; i < shaderDesc.BoundResources; i++)
	{
		// 해당 struct를 통해 binding된 resource name을 기준으로 정보를 가져온다.
		D3D11_SHADER_INPUT_BIND_DESC resource_desc;
		pReflection->GetResourceBindingDesc(i, &resource_desc);

		switch (resource_desc.Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflection->GetConstantBufferByName(resource_desc.Name);
			D3D11_SHADER_BUFFER_DESC cbDesc;
			cBuffer->GetDesc(&cbDesc);

			cbufferRegisterSlot = resource_desc.BindPoint;

			_constantBufferData[resource_desc.Name] = new ConstantBuffer(resource_desc.Name, cbufferRegisterSlot, cbDesc.Size);
		}
		break;
		case D3D_SIT_TEXTURE:
		{
			srvRegisterSlot = resource_desc.BindPoint;

			_shaderResourceViewData[resource_desc.Name] = new ShaderResourceBuffer(resource_desc.Name, srvRegisterSlot);
		}
		break;
		case D3D_SIT_SAMPLER:
		{
			samplerRegisterSlot = resource_desc.BindPoint;

			_samplerStateData[resource_desc.Name] = new SamplerBuffer(resource_desc.Name, samplerRegisterSlot);
		}
		break;
		default:
			break;
		}
	}

	// 사이즈 설정
	_constantBuffers.resize(++cbufferRegisterSlot);
	_shaderResourceViews.resize(++srvRegisterSlot);
	_samplerStates.resize(++samplerRegisterSlot);

	// ShaderReflection 해제
	pReflection->Release();
}

void PixelShader::Update()
{
	// PS에 pixel shader 설정
	Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
		PSSetShader(_ps.Get(), nullptr, 0);

	// PS에 상수 버퍼 설정
	if (!_constantBufferData.empty())
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
			PSSetConstantBuffers(0, (UINT)_constantBuffers.size(), _constantBuffers[0].GetAddressOf());
	}

	// PS에 샘플러 설정
	if (!_samplerStateData.empty())
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
			PSSetSamplers(0, (UINT)_samplerStates.size(), _samplerStates[0].GetAddressOf());
	}

	// PS에 리소스 설정
	if (!_shaderResourceViews.empty())
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
			PSSetShaderResources(0, (UINT)_shaderResourceViews.size(), _shaderResourceViews[0].GetAddressOf());
	}
}

void PixelShader::Release()
{
	_ps->Release();
}
