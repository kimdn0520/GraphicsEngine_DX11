#include "pch.h"
#include "DeferredPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "ViewPort.h"

void DeferredPass::Start()
{
	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		_gBuffers[i] = new RenderTargetView();

		_gBuffers[i]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();	// rtv �����´�.
	}

	_gBuffers[DEFERRED_COUNT - 1] = new RenderTargetView();

	// !!! ��� DXGI_FORMAT_R32_UINT�� ������� SV_Target�� uint�� �����ű⶧����
	_gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32_UINT);

	_gBufferViews[DEFERRED_COUNT - 1] = _gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_screenViewPort = new ViewPort();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}

void DeferredPass::Release()
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		_gBuffers[i]->Release();

		_gBufferViews[i].ReleaseAndGetAddressOf();	// ���� ������?
	}

	_screenViewPort->Release();
}

void DeferredPass::OnResize(int width, int height)
{
	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		_gBuffers[i]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_gBufferViews[i].ReleaseAndGetAddressOf();

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();
	}

	_gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32_UINT);

	_gBufferViews[DEFERRED_COUNT - 1].ReleaseAndGetAddressOf();
	_gBufferViews[DEFERRED_COUNT - 1] = _gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_screenViewPort->OnResize(width, height);
}

void DeferredPass::BeginRender()
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		_gBuffers[i]->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	// G-Buffer���� Ȥ�� ���̴� ���ҽ��� �������� �� ������ Unbind
	ID3D11ShaderResourceView* nullSRV[DEFERRED_COUNT] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, DEFERRED_COUNT, nullSRV);

	g_deviceContext->OMSetRenderTargets(DEFERRED_COUNT, _gBufferViews[0].GetAddressOf(), _gBuffers[0]->GetDepthStencilView().Get());

	_screenViewPort->SetViewPort(g_deviceContext);
}

void DeferredPass::Render(std::vector<ObjectInfo*> mesh)
{
}

void DeferredPass::EndRender()
{
}
