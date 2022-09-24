#include "pch.h"
#include "LightPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "LightManager.h"

void LightPass::Start()
{
	_quad_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Quad_VS"));
	_light_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Light_PS"));
	
	lightingRTV = std::make_shared<RenderTargetView>();

	lightingRTV->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	_screenViewPort = std::make_shared<ViewPort>();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}


void LightPass::Release()
{
	lightingRTV->Release();

	_screenViewPort->Release();

	_quad_VS.reset();
	_light_PS.reset();
}

void LightPass::OnResize(int width, int height)
{
	lightingRTV->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	_screenViewPort->OnResize(width, height);
}

void LightPass::RenderStart()
{
	lightingRTV->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// dsv는 nullptr로 넣으면 된다.
	g_deviceContext->OMSetRenderTargets(1, lightingRTV->GetRenderTargetView().GetAddressOf(), nullptr);

	_screenViewPort->SetViewPort(g_deviceContext);
}

void LightPass::Render(const std::vector<std::shared_ptr<RenderTargetView>> gBuffers, std::shared_ptr<DepthStencilView> shadowDSV)
{
	RenderStart();

	// 버텍스 쉐이더 업데이트
	_quad_VS->Update();

	// 픽셀 쉐이더에 SRV 셋팅 
	_light_PS->SetResourceViewBuffer(gBuffers[0]->GetSRV().Get(), "Depth");
	_light_PS->SetResourceViewBuffer(gBuffers[1]->GetSRV().Get(), "Normal");
	_light_PS->SetResourceViewBuffer(gBuffers[2]->GetSRV().Get(), "Position");
	_light_PS->SetResourceViewBuffer(gBuffers[3]->GetSRV().Get(), "Albedo");
	_light_PS->SetResourceViewBuffer(gBuffers[4]->GetSRV().Get(), "ObjectID");
	_light_PS->SetResourceViewBuffer(shadowDSV->GetShaderResourceView().Get(), "Shadow");

	_light_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");

	// TODO : PBR 에서 제대로 하자 Legacy에서는 일단 대충 안쪽에서 재질 넘기자
	//_light_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

	cbTexture cbTextureBuf;
	cbTextureBuf.textureInfo = Vector4(												// 이녀석으로 텍스쳐의 uint를 뽑으려고 텍스쳐 사이즈 건네줌
		Graphics_Interface::Get()->GetScreenWidth(),
		Graphics_Interface::Get()->GetScreenHeight(),
		1.f / Graphics_Interface::Get()->GetScreenWidth(),
		1.f / Graphics_Interface::Get()->GetScreenHeight());

	_light_PS->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");

	_light_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);

	RenderEnd();
}

void LightPass::RenderEnd()
{
	ID3D11ShaderResourceView* nullSRV[6] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, 6, nullSRV);
}
