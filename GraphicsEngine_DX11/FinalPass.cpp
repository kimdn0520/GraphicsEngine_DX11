#include "pch.h"
#include "FinalPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

void FinalPass::Start()
{
	_quad_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Quad_VS"));
}

void FinalPass::Release()
{

}

void FinalPass::OnResize(int width, int height)
{

}

void FinalPass::RenderStart()
{
	// 렌더링 타겟을 다시 백버퍼로 돌린다.
	Graphics_Interface::Get()->RenderToBackBuffer();

	// z버퍼를 꺼준다.
	Graphics_Interface::Get()->TurnZBufferOff();
}

void FinalPass::Render()
{
	RenderStart();

	_quad_VS->Update();

	g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetRasterState().Get());

	unsigned int stride = ResourceManager::Get()->GetMesh(SCREEN_MESH)->stride;
	unsigned int offset = 0;

	g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetPrimitiveTopology());

	g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

	g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(SCREEN_MESH)->GetIdxBufferSize(), 0, 0);

	RenderEnd();
}

void FinalPass::RenderEnd()
{
	// z버퍼를 켜준다.
	Graphics_Interface::Get()->TurnZBufferOn();
}
