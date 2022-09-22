#include "pch.h"
#include "ShadowPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "LightManager.h"

void ShadowPass::Start()
{
	_shadow_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Shadow_VS"));
	_shadow_Skinned_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Shadow_Skinned_VS"));

	shadowDSV = new DepthStencilView();

	shadowDSV->InitializeShadowMap(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), false);

	_screenViewPort = new ViewPort();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}

void ShadowPass::Release()
{
	shadowDSV->Release();

	_screenViewPort->Release();

	delete _shadow_VS;
	delete _shadow_Skinned_VS;
}

void ShadowPass::OnResize(int width, int height)
{
	shadowDSV->OnResize(g_device ,width, height);

	_screenViewPort->OnResize(width, height);
}

void ShadowPass::RenderStart()
{
	shadowDSV->ClearDepthStencilView(g_deviceContext);

	_screenViewPort->SetViewPort(g_deviceContext);

	// 렌더 대상을 null로 설정함으로써 색상 기록을 비활성화 한다.
	// 그래픽 카드는 깊이만 그리는 작업에 최적화 되었다. 그래서 깊이 전용 렌더링 패스는
	// 색상과 깊이를 모두 그리는 패스에 비해 훨씬 빠르다.
	g_deviceContext->OMSetRenderTargets(0, nullptr, shadowDSV->GetDepthStencilView().Get());
}

void ShadowPass::Render(std::vector<ObjectInfo*> meshs)
{
	RenderStart();

	cbLightViewProj cbLightViewProjBuffer;
	cbLightViewProjBuffer.lightViewProj = LightManager::Get()->cbLightBuffer.gDirLight[0].LightViewProj;

	for (auto& mesh : meshs)
	{
		// 그림자 영향을 받지않는 녀석이면 넘어감
		if (!mesh->isShadow)
			continue;

		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = mesh->worldTM;
			cbMeshBuffer.objectID = mesh->objectID;

			// Skinned Mesh
			if (mesh->isSkinned)
			{
				cbSkinned cbSkinnedBuffer;
				memcpy(&cbSkinnedBuffer.gBoneTransforms, mesh->finalBoneListMatrix, sizeof(Matrix) * 96);
				_shadow_Skinned_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_shadow_Skinned_VS->ConstantBufferUpdate(&cbSkinnedBuffer, "cbSkinned");
				_shadow_Skinned_VS->ConstantBufferUpdate(&cbLightViewProjBuffer, "cbLightViewProj");
				_shadow_Skinned_VS->Update();
			}
			// Static Mesh
			else
			{
				_shadow_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_shadow_VS->ConstantBufferUpdate(&cbLightViewProjBuffer, "cbLightViewProj");
				_shadow_VS->Update();
			}

			g_deviceContext->PSSetShader(nullptr, NULL, NULL);

			if(ResourceManager::Get()->GetMesh(mesh->meshID)->rasterNum == 0)
				g_deviceContext->RSSetState(Graphics_Interface::Get()->GetShadowSolid()->GetrasterizerState().Get());
			else if(ResourceManager::Get()->GetMesh(mesh->meshID)->rasterNum == 1)
				g_deviceContext->RSSetState(Graphics_Interface::Get()->GetShadowWire()->GetrasterizerState().Get());

			unsigned int stride = ResourceManager::Get()->GetMesh(mesh->meshID)->stride;
			unsigned int offset = 0;

			// 토폴로지 설정
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(mesh->meshID)->GetPrimitiveTopology());

			// 버텍스 버퍼 설정
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(mesh->meshID)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// 인덱스 버퍼 설정
			g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(mesh->meshID)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// 그린다
			g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(mesh->meshID)->GetIdxBufferSize(), 0, 0);
		}
		default:
			break;
		}
	}

	RenderEnd();
}

void ShadowPass::RenderEnd()
{
	
}
