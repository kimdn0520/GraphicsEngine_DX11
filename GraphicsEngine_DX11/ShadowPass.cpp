#include "pch.h"
#include "ShadowPass.h"
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

void ShadowPass::Start()
{
	_shadow_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Shadow_VS"));
	_shadow_Skinned_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Shadow_Skinned_VS"));

	_shadowDSV = new DepthStencilView();

	_shadowDSV->InitializeShadowMap(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), false);

	_screenViewPort = new ViewPort();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}

void ShadowPass::Release()
{
	_shadowDSV->Release();

	_screenViewPort->Release();

	delete _shadow_VS;
	delete _shadow_Skinned_VS;
}

void ShadowPass::OnResize(int width, int height)
{
	_shadowDSV->OnResize(g_device ,width, height);

	_screenViewPort->OnResize(width, height);
}

void ShadowPass::RenderStart()
{
	_shadowDSV->ClearDepthStencilView(g_deviceContext);

	_screenViewPort->SetViewPort(g_deviceContext);

	// ���� ����� null�� ���������ν� ���� ����� ��Ȱ��ȭ �Ѵ�.
	// �׷��� ī��� ���̸� �׸��� �۾��� ����ȭ �Ǿ���. �׷��� ���� ���� ������ �н���
	// ����� ���̸� ��� �׸��� �н��� ���� �ξ� ������.
	g_deviceContext->OMSetRenderTargets(0, nullptr, _shadowDSV->GetDepthStencilView().Get());

	//for(auto dirLight : LightManager::Get()->dirLights)
	//{
	//	// Light View Projection Matrix �� �������Ѵ�.
	//	cbLightViewProjBuffer.lightViewProj = XMMatrixLookAtLH()
	//}
}

void ShadowPass::Render(std::vector<ObjectInfo*> meshs)
{
	RenderStart();

	RenderEnd();
}

void ShadowPass::RenderEnd()
{
	
}
