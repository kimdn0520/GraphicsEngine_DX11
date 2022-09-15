#include "pch.h"
#include "DeferredPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

void DeferredPass::Start()
{
	_model_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Model_VS"));
	_model_Skinned_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Model_Skinned_VS"));
	_model_PS = dynamic_cast<PixelShader*>(ShaderManager::Get()->GetShader(L"Model_PS"));

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

void DeferredPass::Render(std::vector<ObjectInfo*> meshs)
{
	for (auto& mesh : meshs)
	{
		for (auto& mat : mesh->materials)
		{
			switch (mesh->type)
			{
			case OBJECT_TYPE::DEFAULT:
			{
				cbMesh cbMeshBuffer;
				cbMeshBuffer.gWorld = mesh->worldTM;
				cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo->viewTM * RenderManager::s_cameraInfo->projTM;
				cbMeshBuffer.objectID = mesh->objectID;
				XMVECTOR det = XMMatrixDeterminant(mesh->worldTM);
				cbMeshBuffer.gWorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, mesh->worldTM));

				// Skinned Mesh
				if (mesh->isSkinned)
				{
					cbSkinned cbSkinnedBuffer;
					memcpy(&cbSkinnedBuffer.gBoneTransforms, mesh->finalBoneListMatrix, sizeof(Matrix) * 96);
					_model_Skinned_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
					_model_Skinned_VS->ConstantBufferUpdate(&cbSkinnedBuffer, "cbSkinned");
					_model_Skinned_VS->Update();
				}
				// Static Mesh
				else
				{
					_model_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
					_model_VS->Update();
				}

				cbMaterial cbMaterialBuffer;
				cbMaterialBuffer.gMaterialAmbient = mat->ambient;
				cbMaterialBuffer.gMaterialDiffuse = mat->diffuse;
				cbMaterialBuffer.gMaterialSpecular = mat->specular;
				cbMaterialBuffer.gMaterialReflection = mat->reflection;
				cbMaterialBuffer.isDiffuseTexture = mat->isDiffuse;
				cbMaterialBuffer.isNormalTexture = mat->isNormal;
				cbMaterialBuffer.isSpecularTexture = mat->isSpecular;

				if (mat->isDiffuse)
					_model_PS->SetResourceViewBuffer(mat->diffuseTexture, "gDiffuseMap");

				if (mat->isNormal)
					_model_PS->SetResourceViewBuffer(mat->normalTexture, "gNormalMap");

				if (mat->isSpecular)
					_model_PS->SetResourceViewBuffer(mat->specularTexture, "gSpecularMap");
				
				_model_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_model_PS->ConstantBufferUpdate(&cbMaterialBuffer, "cbMaterial");

				_model_PS->Update();

				g_deviceContext->RSSetState(ResourceManager::Get()->GetMesh(mesh->meshID)->GetRasterState().Get());

				unsigned int stride = ResourceManager::Get()->GetMesh(mesh->meshID)->stride;
				unsigned int offset = 0;

				g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(mesh->meshID)->GetPrimitiveTopology());

				g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(mesh->meshID)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				g_deviceContext->DrawIndexed(ResourceManager::Get()->GetMesh(mesh->meshID)->GetIdxBufferSize(), 0, 0);
			}
			case OBJECT_TYPE::SKY_BOX:
			{

			}
			default:
				break;
			}
		}
	}
}

void DeferredPass::EndRender()
{
	//// ������ Ÿ���� �ٽ� ����۷� ������.
	//GraphicsEngineAPI::GetInstance()->SetBackBufferRenderTarget();

	//// z���۸� ���ش�.
	//GraphicsEngineAPI::GetInstance()->TurnZBufferOff();

	//// left ~ right : -1.0f ~ 1.0f ����..!
	//// top ~ bottom : 1.0f ~ -1.0f ����..!
	//_deferredWindow->Render(g_deviceContext, Vector4(-1.0f, 1.0f, 1.0f, -1.0f));	// ũ�� ��� ����

	//// Solid�� ����
	//g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());

	//// ���ؽ� ���̴� ������Ʈ
	//_deferred_VS->Update();

	//// �ȼ� ���̴��� SRV ���� 
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[0]->GetSRV().Get(), "Albedo");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[1]->GetSRV().Get(), "Depth");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[2]->GetSRV().Get(), "Normal");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[3]->GetSRV().Get(), "Position");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[4]->GetSRV().Get(), "MaterialID");		// ���� ������ �ȹ����� // �̸��� �ٲ��ش�.

	///*_deferred_PS->SetResourceViewBuffer(_gReflectionSkySRV, "ReflectionSkyTexture");
	//_deferred_PS->SetResourceViewBuffer(_gReflectionObjectSRV, "ReflectionObjectTexture");*/

	////_deferred_PS->SetResourceViewBuffer(ResourceManager::)

	//_deferred_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
	//_deferred_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

	//_cbTextureBuf.textureInfo = Vector4(												// �̳༮���� �ؽ����� uint�� �������� �ؽ��� ������ �ǳ���
	//	static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().width),
	//	static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().height),
	//	1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, 1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().height);
	//_deferred_PS->ConstantBufferUpdate(&_cbTextureBuf, "cbTexture");

	//_deferred_PS->Update();

	//// �׸���!
	//g_deviceContext->DrawIndexed(_deferredWindow->GetIndexCount(), 0, 0);

	//// z���� �ٽ� ���ش�.
	//GraphicsEngineAPI::GetInstance()->TurnZBufferOn();
}