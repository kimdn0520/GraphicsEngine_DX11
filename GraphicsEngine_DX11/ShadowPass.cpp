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
	_shadow_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Shadow_VS"));
	_shadow_Skinned_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Shadow_Skinned_VS"));

	shadowDSV = std::make_shared<DepthStencilView>();

	shadowDSV->InitializeShadowMap(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), false);

	_screenViewPort = std::make_shared<ViewPort>();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}

void ShadowPass::Release()
{
	shadowDSV->Release();

	_screenViewPort->Release();
}

void ShadowPass::OnResize(int width, int height)
{
	shadowDSV->OnResize(g_device ,width, height);

	_screenViewPort->OnResize(width, height);
}

void ShadowPass::RenderStart()
{
	ID3D11ShaderResourceView* nullSRV[5] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, 5, nullSRV);

	shadowDSV->ClearDepthStencilView(g_deviceContext);

	_screenViewPort->SetViewPort(g_deviceContext);

	// ���� ����� null�� ���������ν� ���� ����� ��Ȱ��ȭ �Ѵ�.
	// �׷��� ī��� ���̸� �׸��� �۾��� ����ȭ �Ǿ���. �׷��� ���� ���� ������ �н���
	// ����� ���̸� ��� �׸��� �н��� ���� �ξ� ������.
	g_deviceContext->OMSetRenderTargets(0, nullptr, shadowDSV->GetDepthStencilView().Get());
}

void ShadowPass::Render(std::vector<std::shared_ptr<ObjectInfo>> meshs)
{
	RenderStart();

	Matrix lightView = XMMatrixLookAtLH(Vector3(0, 10, 0), Vector3(0.5, -0.5, 0.5), Vector3(0.0f, 1.0f, 0.0f));
	/*Vector3 right;
	Vector3 up;
	Vector3 look = LightManager::Get()->cbLightBuffer.gDirLight[0].Direction;		
	Vector3 lightPos = Vector3(0, 10, 0);

	look.Normalize();
	up = Vector3(0.f, 1.f, 0.f);
	right = up.Cross(look);
	right.Normalize();

	if (right.Length() <= 0.009f)
	{
		right = Vector3(0.f, 1.f, 2.f).Cross(look);
	}

	right.Normalize();
	up = look.Cross(right);
	up.Normalize();

	float x = -lightPos.Dot(right);
	float y = -lightPos.Dot(up);
	float z = -lightPos.Dot(look);
	 
	Matrix lightView
	{
		right.x, up.x, look.x, 0.f,
		right.y, up.y, look.y, 0.f,
		right.z, up.z, look.z, 0.f,
		x, y, z, 1.0f
	};*/
	
	Matrix lightProj = XMMatrixOrthographicLH(10, 10, 0, 20);

	// ���⼭ ����..
	cbLightViewProj cbLightViewProjBuffer;
	cbLightViewProjBuffer.lightViewProj = lightView * lightProj;

	// directional light�� lightviewproj �־��ش�
	LightManager::Get()->cbLightBuffer.gDirLight[0].LightViewProj = cbLightViewProjBuffer.lightViewProj;

	for (auto& mesh : meshs)
	{
		// �׸��� ������ �����ʴ� �༮�̸� �Ѿ
		if (!mesh->isShadow)
			continue;

		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			cbPerObject cbPerObejctBuffer;
			cbPerObejctBuffer.gWorld = mesh->worldTM;
			cbPerObejctBuffer.objectID = mesh->objectID;

			// Skinned Mesh
			if (mesh->isSkinned)
			{
				cbSkinned cbSkinnedBuffer;
				memcpy(&cbSkinnedBuffer.gBoneTransforms, mesh->finalBoneListMatrix, sizeof(Matrix) * 96);
				_shadow_Skinned_VS->ConstantBufferUpdate(&cbPerObejctBuffer, "cbPerObject");
				_shadow_Skinned_VS->ConstantBufferUpdate(&cbSkinnedBuffer, "cbSkinned");
				_shadow_Skinned_VS->ConstantBufferUpdate(&cbLightViewProjBuffer, "cbLightViewProj");
				_shadow_Skinned_VS->Update();
			}
			// Static Mesh
			else
			{
				_shadow_VS->ConstantBufferUpdate(&cbPerObejctBuffer, "cbPerObject");
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

			// �������� ����
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(mesh->meshID)->GetPrimitiveTopology());

			// ���ؽ� ���� ����
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(mesh->meshID)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// �ε��� ���� ����
			g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(mesh->meshID)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// �׸���
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
