#include "pch.h"
#include "VertexShader.h"
#include "Graphics_Interface.h"
#include "ShaderResource.h"
#include "Device.h"

VertexShader::VertexShader(const std::wstring& shaderName, const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro)
	: ShaderBase(SHADER_TYPE::VERTEX, shaderName),
	_vs(nullptr), _inputLayout(nullptr)
{
	CreateShader(path + shaderName, entryPoint, macro);		// 객체가 생성될때 쉐이더를 만든다.
}

VertexShader::~VertexShader()
{}

void VertexShader::CreateShader(const std::wstring& path, std::string entryPoint, const D3D_SHADER_MACRO* macro)
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	ID3D11ShaderReflection* pReflection = nullptr;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescArray;		// inputlayout요소들을 임시적으로 담음

	DWORD shaderFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		// 전치해서 넣어줌
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	// 정점 쉐이더코드를 컴파일 한다.
	HRESULT hr = D3DCompileFromFile(path.c_str(), macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), "vs_5_0", shaderFlags, NULL, &compiledShader, &compilationMsgs);

	if (FAILED(hr))
		MessageBox(0, TEXT("vertexshader Failed."), 0, 0);

	if (compilationMsgs != 0)
		compilationMsgs->Release();

	// vertex shader 생성
	Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
		CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, _vs.GetAddressOf());

	// reflection 생성
	D3DReflect(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection);

	D3D11_SHADER_DESC shaderDesc;			// 해당 구조체를 통해 ShaderReflection에서 Shader 정보를 가져온다.
	pReflection->GetDesc(&shaderDesc);

	for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC inputElementDesc;
		inputElementDesc.SemanticName = paramDesc.SemanticName;
		inputElementDesc.SemanticIndex = paramDesc.SemanticIndex;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;

		inputElementDescArray.push_back(inputElementDesc);
	}

	// inputLayout 생성
	Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
		CreateInputLayout(&inputElementDescArray[0], (UINT)inputElementDescArray.size(), compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), _inputLayout.GetAddressOf());

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

			_constantBufferData[resource_desc.Name] = std::make_shared<ConstantBuffer>(resource_desc.Name, cbufferRegisterSlot, cbDesc.Size);
		}
		break;
		case D3D_SIT_TEXTURE:
		{
			srvRegisterSlot = resource_desc.BindPoint;

			_shaderResourceViewData[resource_desc.Name] = std::make_shared<ShaderResourceBuffer>(resource_desc.Name, srvRegisterSlot);
		}
		break;
		case D3D_SIT_SAMPLER:
		{
			samplerRegisterSlot = resource_desc.BindPoint;

			_samplerStateData[resource_desc.Name] = std::make_shared<SamplerBuffer>(resource_desc.Name, samplerRegisterSlot);
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

void VertexShader::Update()
{
	// VS에 vertex shader 설정
	Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
		VSSetShader(_vs.Get(), nullptr, 0);

	// VS에 상수 버퍼 설정
	if (!_constantBufferData.empty())
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
			VSSetConstantBuffers(0, (UINT)_constantBuffers.size(), _constantBuffers[0].GetAddressOf());
	}

	// VS에 샘플러 설정
	if (!_samplerStateData.empty())
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
			VSSetSamplers(0, (UINT)_samplerStates.size(), _samplerStates[0].GetAddressOf());
	}

	// VS에 리소스 설정
	if (!_shaderResourceViewData.empty())
	{
		Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
			VSSetShaderResources(0, (UINT)_shaderResourceViews.size(), _shaderResourceViews[0].GetAddressOf());
	}

	// inputLayout 설정
	Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->
		IASetInputLayout(_inputLayout.Get());
}

void VertexShader::Release()
{
	_vs->Release();
	_inputLayout->Release();
}
