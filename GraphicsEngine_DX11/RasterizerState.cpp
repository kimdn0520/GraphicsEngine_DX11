#include "pch.h"
#include "RasterizerState.h"
#include "Device.h"

void RasterizerState::Initialize(std::shared_ptr<Device> device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, int depthBias , float slopeScaledDepthBias)
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.FillMode = fillMode;
	rasterDesc.CullMode = cullMode;

	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthBias = depthBias;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = slopeScaledDepthBias;

	device->GetDevice()->CreateRasterizerState(&rasterDesc, _rasterizerState.GetAddressOf());
}

void RasterizerState::Release()
{
	_rasterizerState.ReleaseAndGetAddressOf();
}

