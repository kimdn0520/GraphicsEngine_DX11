#include "pch.h"
#include "GaussianBlurPass.h"
#include "Graphics_Interface.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ViewPort.h"
#include "RenderTargetView.h"
#include "ResourceManager.h"

void GaussianBlur::Start()
{
	blurOutput = std::make_shared<RenderTargetView>();

	blurOutput->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	blurX_Texture = std::make_shared<RenderTargetView>();

	blurX_Texture->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	quad_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Quad_VS"));
	sampling_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Sampling_PS"));
	blurX_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"BlurX_PS"));
	blurY_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"BlurY_PS"));

	halfViewPort = std::make_shared<ViewPort>();
	halfViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 2, Graphics_Interface::Get()->GetScreenHeight() / 2);

	downHalfTexture = std::make_shared<RenderTargetView>();
	downHalfTexture->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 2, Graphics_Interface::Get()->GetScreenHeight() / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);

	quarterViewPort = std::make_shared<ViewPort>();
	quarterViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 4, Graphics_Interface::Get()->GetScreenHeight() / 4);

	downQuarterTexture = std::make_shared<RenderTargetView>();
	downQuarterTexture->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 4, Graphics_Interface::Get()->GetScreenHeight() / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	octarViewPort = std::make_shared<ViewPort>();
	octarViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 8, Graphics_Interface::Get()->GetScreenHeight() / 8);

	downOctarTexture = std::make_shared<RenderTargetView>();
	downOctarTexture->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 8, Graphics_Interface::Get()->GetScreenHeight() / 8, DXGI_FORMAT_R32G32B32A32_FLOAT);

	sixteenViewPort = std::make_shared<ViewPort>();
	sixteenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth() / 16, Graphics_Interface::Get()->GetScreenHeight() / 16);

	downSixteenTexture = std::make_shared<RenderTargetView>();
	downSixteenTexture->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth() / 16, Graphics_Interface::Get()->GetScreenHeight() / 16, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void GaussianBlur::Release()
{
	blurOutput->Release();

	blurX_Texture->Release();

	quad_VS->Release();

	blurX_PS->Release();

	blurY_PS->Release();

	halfViewPort->Release();

	downHalfTexture->Release();

	quarterViewPort->Release();

	downQuarterTexture->Release();

	octarViewPort->Release();

	downOctarTexture->Release();

	sixteenViewPort->Release();

	downSixteenTexture->Release();
}

void GaussianBlur::OnResize(int width, int height)
{
	blurOutput->Release();

	blurOutput->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	blurX_Texture->Release();

	blurX_Texture->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// move를 하면 새로 메모리를 할당하지 않아도 되고 이미 메모리에 할당된것을 소유권만 넘겨주기때문에 copy동작보다 빠르다.
	auto&& halfWidth = std::move(width / 2);
	auto&& halfHeight = std::move(height / 2);

	halfViewPort->OnResize(halfWidth, halfHeight);
	downHalfTexture->Release();
	downHalfTexture->RenderTargetTextureInit(g_device, halfWidth, halfHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	auto&& quarterWidth = std::move(width / 4);
	auto&& quarterHeight = std::move(height / 4);

	quarterViewPort->OnResize(quarterWidth, quarterHeight);
	downQuarterTexture->Release();
	downQuarterTexture->RenderTargetTextureInit(g_device, quarterWidth, quarterHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	auto&& octarWidth = std::move(width / 8);
	auto&& octarHeight = std::move(height / 8);

	octarViewPort->OnResize(octarWidth, octarHeight);
	downOctarTexture->Release();
	downOctarTexture->RenderTargetTextureInit(g_device, octarWidth, octarHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	auto&& sixteenWidth = std::move(width / 16);
	auto&& sixteenHeight = std::move(height / 16);

	sixteenViewPort->OnResize(sixteenWidth, sixteenHeight);
	downSixteenTexture->Release();
	downSixteenTexture->RenderTargetTextureInit(g_device, sixteenWidth, sixteenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void GaussianBlur::RenderStart(std::shared_ptr<RenderTargetView>& screen)
{
	if (screen->GetWidth() != Graphics_Interface::Get()->GetScreenWidth() || screen->GetHeight() != Graphics_Interface::Get()->GetScreenHeight())
		OnResize(screen->GetWidth(), screen->GetHeight());
}

void GaussianBlur::Render(std::shared_ptr<RenderTargetView>& screen)
{
	RenderStart(screen);

	/// <summary>
	/// 수평 블러
	/// </summary>
	blurX_Texture->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, blurX_Texture->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	cbTexture cbTextureBuf;
	cbTextureBuf.textureInfo = Vector4(
		Graphics_Interface::Get()->GetScreenWidth(),
		Graphics_Interface::Get()->GetScreenHeight(),
		1.f / Graphics_Interface::Get()->GetScreenWidth(),
		1.f / Graphics_Interface::Get()->GetScreenHeight());

	blurX_PS->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");
	blurX_PS->SetResourceViewBuffer(screen->GetSRV().Get(), "TargetTexture");

	blurX_PS->Update();
	
	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);

	/// <summary>
	/// 수직 블러
	/// </summary>
	blurOutput->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, blurOutput->GetRenderTargetView().GetAddressOf(), nullptr);

	quad_VS->Update();

	blurY_PS->ConstantBufferUpdate(&cbTextureBuf, "cbTexture");
	blurY_PS->SetResourceViewBuffer(blurX_Texture->GetSRV().Get(), "TargetTexture");

	blurY_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);

	RenderEnd();
}

void GaussianBlur::RenderEnd()
{
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, 1, nullSRV);
}

void GaussianBlur::DownSampling2x(std::shared_ptr<RenderTargetView>& screen)
{
	downHalfTexture->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, downHalfTexture->GetRenderTargetView().GetAddressOf(), nullptr);

	halfViewPort->SetViewPort(g_deviceContext);

	quad_VS->Update();

	sampling_PS->SetResourceViewBuffer(screen->GetSRV().Get(), "Sampling_PS");

	sampling_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void GaussianBlur::DownSampling4x(std::shared_ptr<RenderTargetView>& screen)
{
	downQuarterTexture->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, downQuarterTexture->GetRenderTargetView().GetAddressOf(), nullptr);

	quarterViewPort->SetViewPort(g_deviceContext);

	quad_VS->Update();

	sampling_PS->SetResourceViewBuffer(screen->GetSRV().Get(), "Sampling_PS");

	sampling_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void GaussianBlur::DownSampling8x(std::shared_ptr<RenderTargetView>& screen)
{
	downOctarTexture->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, downOctarTexture->GetRenderTargetView().GetAddressOf(), nullptr);

	octarViewPort->SetViewPort(g_deviceContext);

	quad_VS->Update();

	sampling_PS->SetResourceViewBuffer(screen->GetSRV().Get(), "Sampling_PS");

	sampling_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}

void GaussianBlur::DownSampling16x(std::shared_ptr<RenderTargetView>& screen)
{
	downSixteenTexture->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	g_deviceContext->OMSetRenderTargets(1, downSixteenTexture->GetRenderTargetView().GetAddressOf(), nullptr);

	sixteenViewPort->SetViewPort(g_deviceContext);

	quad_VS->Update();

	sampling_PS->SetResourceViewBuffer(screen->GetSRV().Get(), "Sampling_PS");

	sampling_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}
