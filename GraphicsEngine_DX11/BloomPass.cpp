#include "pch.h"
#include "BloomPass.h"
#include "Graphics_Interface.h"
#include "GaussianBlurPass.h"
#include "RenderTargetView.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ViewPort.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

void BloomPass::Start()
{
	screenViewPort = std::make_shared<ViewPort>();
	screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());

	blurPass = std::make_shared<GaussianBlurPass>();
	blurPass->Start();

	bloomSample = std::make_shared<RenderTargetView>();
	bloomSample->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	bloomOutput = std::make_shared<RenderTargetView>();
	bloomOutput->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	quad_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Quad_VS"));
	bloomCurve_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"BloomCurve_PS"));
	bloomAddUp_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"BloomAddUp_PS"));

	octarViewPort = std::make_shared<ViewPort>();
	octarViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 8, Graphics_Interface::Get()->GetScreenHeight() / 8);

	octarTarget = std::make_shared<RenderTargetView>();
	octarTarget->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 8, Graphics_Interface::Get()->GetScreenHeight() / 8, DXGI_FORMAT_R32G32B32A32_FLOAT);

	quarterViewPort = std::make_shared<ViewPort>();
	quarterViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 4, Graphics_Interface::Get()->GetScreenHeight() / 4);

	quarterTarget = std::make_shared<RenderTargetView>();
	quarterTarget->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 4, Graphics_Interface::Get()->GetScreenHeight() / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	halfViewPort = std::make_shared<ViewPort>();
	halfViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 2, Graphics_Interface::Get()->GetScreenHeight() / 2);

	halfTarget = std::make_shared<RenderTargetView>();
	halfTarget->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 2, Graphics_Interface::Get()->GetScreenHeight() / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void BloomPass::Release()
{
	 screenViewPort->Release();

	 blurPass->Release();

	 bloomSample->Release();

	 bloomOutput->Release();

	 quad_VS->Release();
	 
	 bloomCurve_PS->Release();

	 bloomAddUp_PS->Release();

	 octarViewPort->Release();

	 octarTarget->Release();

	 quarterViewPort->Release();

	 quarterTarget->Release();

	 halfViewPort->Release();

	 halfTarget->Release();
}

void BloomPass::OnResize(int width, int height)
{
	screenViewPort->OnResize(width, height);

	blurPass->OnResize(width, height);

	bloomSample->Release();

	bloomSample->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	bloomOutput->Release();

	bloomOutput->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	auto&& octarWidth = std::move(width / 8);
	auto&& octarHeight = std::move(height / 8);

	octarViewPort->OnResize(octarWidth, octarHeight);
	octarTarget->Release();
	octarTarget->RenderTargetTextureInit(g_device, octarWidth, octarHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	auto&& quarterWidth = std::move(width / 4);
	auto&& quarterHeight = std::move(height / 4);

	quarterViewPort->OnResize(quarterWidth, quarterHeight);
	quarterTarget->Release();
	quarterTarget->RenderTargetTextureInit(g_device, quarterWidth, quarterHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	auto&& halfWidth = std::move(width / 2);
	auto&& halfHeight = std::move(height / 2);

	halfViewPort->OnResize(halfWidth, halfHeight);
	halfTarget->Release();
	halfTarget->RenderTargetTextureInit(g_device, halfWidth, halfHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

}

void BloomPass::Render(std::shared_ptr<RenderTargetView> targetScreen)
{
	// 1. targetScreen(emissive texture)를 bloomCurve_PS를 써서 bloomSample에 그려준다.
	// 2. bloomSample을 다운샘플링 해준다.
	// 3. 다운샘플링 Blur 및 합성 작업..
	
	BloomCurveTarget(targetScreen);

	blurPass->ProcessDownSampling(targetScreen);

	Blur16XAddUp8X();

	Blur8XAddUp4X();

	Blur4XAddUp2X();

	Blur2XAddUpOrigin(targetScreen);

	RenderEnd();
}

void BloomPass::RenderEnd()
{
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, 3, nullSRV);
}

void BloomPass::BloomCurveTarget(std::shared_ptr<RenderTargetView> targetScreen)
{
	screenViewPort->SetViewPort(g_deviceContext);

	bloomSample->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, bloomSample->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	bloomCurve_PS->SetResourceViewBuffer(targetScreen->GetSRV().Get(), "TargetTexture");
	bloomCurve_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void BloomPass::Blur16XAddUp8X()
{
	blurPass->Render(blurPass->downSixteenTexture);

	octarViewPort->SetViewPort(g_deviceContext);

	octarTarget->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, octarTarget->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	bloomAddUp_PS->SetResourceViewBuffer(blurPass->blurOutput->GetSRV().Get(), "BlurOutput");
	bloomAddUp_PS->SetResourceViewBuffer(blurPass->downOctarTexture->GetSRV().Get(), "AddUpTexture");
	bloomAddUp_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void BloomPass::Blur8XAddUp4X()
{
	blurPass->Render(octarTarget);

	quarterViewPort->SetViewPort(g_deviceContext);

	quarterTarget->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, quarterTarget->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	bloomAddUp_PS->SetResourceViewBuffer(blurPass->blurOutput->GetSRV().Get(), "BlurOutput");
	bloomAddUp_PS->SetResourceViewBuffer(blurPass->downQuarterTexture->GetSRV().Get(), "AddUpTexture");
	bloomAddUp_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void BloomPass::Blur4XAddUp2X()
{
	blurPass->Render(quarterTarget);

	halfViewPort->SetViewPort(g_deviceContext);

	halfTarget->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, halfTarget->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	bloomAddUp_PS->SetResourceViewBuffer(blurPass->blurOutput->GetSRV().Get(), "BlurOutput");
	bloomAddUp_PS->SetResourceViewBuffer(blurPass->downHalfTexture->GetSRV().Get(), "AddUpTexture");
	bloomAddUp_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void BloomPass::Blur2XAddUpOrigin(std::shared_ptr<RenderTargetView> targetScreen)
{
	blurPass->Render(halfTarget);

	screenViewPort->SetViewPort(g_deviceContext);

	bloomOutput->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, bloomOutput->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	bloomAddUp_PS->SetResourceViewBuffer(blurPass->blurOutput->GetSRV().Get(), "BlurOutput");
	bloomAddUp_PS->SetResourceViewBuffer(targetScreen->GetSRV().Get(), "AddUpTexture");
	bloomAddUp_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}
