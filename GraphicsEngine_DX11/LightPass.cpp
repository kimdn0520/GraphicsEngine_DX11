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

	_light_PBR_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Light_PBR_PS"));
	_light_PBR_PS1 = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Light_PBR_PS1"));
	_light_PBR_PS2 = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Light_PBR_PS2"));
	_light_PBR_PS3 = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Light_PBR_PS3"));
	
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
	_light_PBR_PS.reset();
	_light_PBR_PS1.reset();
	_light_PBR_PS2.reset();
	_light_PBR_PS3.reset();
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

void LightPass::Render(const std::vector<std::shared_ptr<RenderTargetView>> gBuffers, std::shared_ptr<DepthStencilView> shadowDSV, std::shared_ptr<RenderTargetView> ssaoMap)
{
	RenderStart();

	// 버텍스 쉐이더 업데이트
	_quad_VS->Update();
	
	cbTexture cbTextureBuf;
	cbTextureBuf.textureInfo = Vector4(												
		Graphics_Interface::Get()->GetScreenWidth(),
		Graphics_Interface::Get()->GetScreenHeight(),
		1.f / Graphics_Interface::Get()->GetScreenWidth(),
		1.f / Graphics_Interface::Get()->GetScreenHeight());

	// 와 이걸 안해주고있었네 카메라posW 넘겨줘야함..
	LightManager::cbLightBuffer.gEyePosW = RenderManager::s_cameraInfo->worldPos;

	// 픽셀 쉐이더에 SRV 셋팅 
	if (ssaoMap != nullptr && isShadow)
	{
		_light_PBR_PS3->SetResourceViewBuffer(gBuffers[0]->GetSRV().Get(), "Albedo");
		_light_PBR_PS3->SetResourceViewBuffer(gBuffers[1]->GetSRV().Get(), "DMRAO");
		_light_PBR_PS3->SetResourceViewBuffer(gBuffers[2]->GetSRV().Get(), "Normal");
		_light_PBR_PS3->SetResourceViewBuffer(gBuffers[3]->GetSRV().Get(), "Position");
		_light_PBR_PS3->SetResourceViewBuffer(gBuffers[4]->GetSRV().Get(), "Emissive");
		_light_PBR_PS3->AddShaderResourceViewData("Shadow", 5);
		_light_PBR_PS3->AddShaderResourceViewData("SSAO", 6);
		_light_PBR_PS3->SetResourceViewBuffer(shadowDSV->GetShaderResourceView().Get(), "Shadow");
		_light_PBR_PS3->SetResourceViewBuffer(ssaoMap->GetSRV().Get(), "SSAO");
		_light_PBR_PS3->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
		_light_PBR_PS3->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");

		_light_PBR_PS3->Update();
	}
	else if (ssaoMap != nullptr && !isShadow)
	{
		_light_PBR_PS2->SetResourceViewBuffer(gBuffers[0]->GetSRV().Get(), "Albedo");
		_light_PBR_PS2->SetResourceViewBuffer(gBuffers[1]->GetSRV().Get(), "DMRAO");
		_light_PBR_PS2->SetResourceViewBuffer(gBuffers[2]->GetSRV().Get(), "Normal");
		_light_PBR_PS2->SetResourceViewBuffer(gBuffers[3]->GetSRV().Get(), "Position");
		_light_PBR_PS2->SetResourceViewBuffer(gBuffers[4]->GetSRV().Get(), "Emissive");
		_light_PBR_PS2->AddShaderResourceViewData("SSAO", 6);
		_light_PBR_PS2->SetResourceViewBuffer(ssaoMap->GetSRV().Get(), "SSAO");
		_light_PBR_PS2->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
		//_light_PBR_PS2->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");

		_light_PBR_PS2->Update();
	}
	else if(ssaoMap == nullptr && isShadow)
	{
		_light_PBR_PS1->SetResourceViewBuffer(gBuffers[0]->GetSRV().Get(), "Albedo");
		_light_PBR_PS1->SetResourceViewBuffer(gBuffers[1]->GetSRV().Get(), "DMRAO");
		_light_PBR_PS1->SetResourceViewBuffer(gBuffers[2]->GetSRV().Get(), "Normal");
		_light_PBR_PS1->SetResourceViewBuffer(gBuffers[3]->GetSRV().Get(), "Position");
		_light_PBR_PS1->SetResourceViewBuffer(gBuffers[4]->GetSRV().Get(), "Emissive");
		_light_PBR_PS1->AddShaderResourceViewData("Shadow", 5);
		_light_PBR_PS1->SetResourceViewBuffer(shadowDSV->GetShaderResourceView().Get(), "Shadow");
		_light_PBR_PS1->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
		_light_PBR_PS1->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");
		
		_light_PBR_PS1->Update();
	}
	else if (ssaoMap == nullptr && !isShadow)
	{
		/*_light_PBR_PS->AddShaderResourceViewData("DMRAO", 0);
		_light_PBR_PS->AddShaderResourceViewData("Normal", 1);
		_light_PBR_PS->AddShaderResourceViewData("Position", 2);
		_light_PBR_PS->AddShaderResourceViewData("Albedo", 3);
		_light_PBR_PS->AddShaderResourceViewData("Emissive", 4);*/
		_light_PBR_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
		_light_PBR_PS->SetResourceViewBuffer(gBuffers[0]->GetSRV().Get(), "Albedo");
		_light_PBR_PS->SetResourceViewBuffer(gBuffers[1]->GetSRV().Get(), "DMRAO");
		_light_PBR_PS->SetResourceViewBuffer(gBuffers[2]->GetSRV().Get(), "Normal");
		_light_PBR_PS->SetResourceViewBuffer(gBuffers[3]->GetSRV().Get(), "Position");
		_light_PBR_PS->SetResourceViewBuffer(gBuffers[4]->GetSRV().Get(), "Emissive");
		//_light_PBR_PS->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");

		_light_PBR_PS->Update();
	}

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
