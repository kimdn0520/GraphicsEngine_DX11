#include "pch.h"
#include "DebugPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredPass.h"
#include "ShadowPass.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

void DebugPass::Start()
{
	_quad_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Quad_VS"));
	_debug_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Debug_PS"));

	_deferredPass = RenderManager::Get()->GetDeferredpass();
	_shadowPass = RenderManager::Get()->GetShadowPass();
}

void DebugPass::Release()
{
	_quad_VS.reset();
	_debug_PS.reset();
}

void DebugPass::OnResize(int width, int height)
{
	
}

void DebugPass::Render()
{
	_quad_VS->Update();

	_debug_PS->SetResourceViewBuffer(_deferredPass->gBuffers[0]->GetSRV().Get(), "Albedo");
	_debug_PS->SetResourceViewBuffer(_deferredPass->gBuffers[1]->GetSRV().Get(), "Depth");
	_debug_PS->SetResourceViewBuffer(_deferredPass->gBuffers[2]->GetSRV().Get(), "Normal");
	_debug_PS->SetResourceViewBuffer(_deferredPass->gBuffers[3]->GetSRV().Get(), "Position");
	_debug_PS->SetResourceViewBuffer(_shadowPass->shadowDSV->GetShaderResourceView().Get(), "Shadow");
	_debug_PS->SetResourceViewBuffer(_deferredPass->gBuffers[4]->GetSRV().Get(), "Emissive");

	_debug_PS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(DEBUG_SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(DEBUG_SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(DEBUG_SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(DEBUG_SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(DEBUG_SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(DEBUG_SCREEN_MESH)->GetIdxBufferSize(), 0, 0);
}