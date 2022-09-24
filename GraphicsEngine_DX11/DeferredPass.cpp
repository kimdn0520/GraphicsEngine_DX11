#include "pch.h"
#include "DeferredPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

void DeferredPass::Start()
{
	_model_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Model_VS"));
	_model_Skinned_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Model_Skinned_VS"));
	_model_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Model_PS"));

	_quad_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Quad_VS"));

	gBuffers.resize(DEFERRED_COUNT);

	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		gBuffers[i] = std::make_shared<RenderTargetView>();

		gBuffers[i]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

		_gBufferViews[i] = gBuffers[i]->GetRenderTargetView();	// rtv 가져온다.
	}

	gBuffers[DEFERRED_COUNT - 1] = std::make_shared<RenderTargetView>();

	// !!! 얘는 DXGI_FORMAT_R32_UINT로 해줘야해 SV_Target을 uint로 뽑을거기때문에
	gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32_UINT);

	_gBufferViews[DEFERRED_COUNT - 1] = gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_deferredDSV = std::make_shared<DepthStencilView>();

	_deferredDSV->Initialize(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());

	_screenViewPort = std::make_shared<ViewPort>();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}

void DeferredPass::Release()
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		gBuffers[i]->Release();

		_gBufferViews[i].ReleaseAndGetAddressOf();	// 오류 나려나?
	}

	_deferredDSV->Release();

	_screenViewPort->Release();
	
	_quad_VS.reset();
	_model_VS.reset();
	_model_Skinned_VS.reset();
	_model_PS.reset();
}

void DeferredPass::OnResize(int width, int height)
{
	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		gBuffers[i]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_gBufferViews[i].ReleaseAndGetAddressOf();

		_gBufferViews[i] = gBuffers[i]->GetRenderTargetView();
	}

	gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32_UINT);

	_gBufferViews[DEFERRED_COUNT - 1].ReleaseAndGetAddressOf();
	_gBufferViews[DEFERRED_COUNT - 1] = gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_deferredDSV->OnResize(g_device, width, height);

	_screenViewPort->OnResize(width, height);
}

void DeferredPass::RenderStart()
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		gBuffers[i]->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	_deferredDSV->ClearDepthStencilView(g_deviceContext);

	// G-Buffer들이 혹시 쉐이더 리소스로 박혀있을 수 있으니 Unbind
	ID3D11ShaderResourceView* nullSRV[DEFERRED_COUNT + 1] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, DEFERRED_COUNT + 1, nullSRV);

	g_deviceContext->OMSetRenderTargets(DEFERRED_COUNT, _gBufferViews[0].GetAddressOf(), _deferredDSV->GetDepthStencilView().Get());

	_screenViewPort->SetViewPort(g_deviceContext);
}

void DeferredPass::Render(std::vector<std::shared_ptr<ObjectInfo>> meshs, std::shared_ptr<DepthStencilView> shadowDSV)
{
	RenderStart();

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
				cbMaterialBuffer.isLight = mat->isLight;

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

				// 토폴로지 설정
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::Get()->GetMesh(mesh->meshID)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::Get()->GetMesh(mesh->meshID)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::Get()->GetMesh(mesh->meshID)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다
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

