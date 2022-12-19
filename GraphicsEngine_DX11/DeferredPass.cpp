#include "pch.h"
#include "DeferredPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

void DeferredPass::Start()
{
	_model_PBR_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Model_PBR_VS"));
	_model_PBR_Skinned_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Model_PBR_Skinned_VS"));
	//_model_PBR_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Model_PBR_PS"));

	_skybox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::Get()->GetShader(L"Skybox_VS"));
	_skybox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(L"Skybox_PS"));

	gBuffers.resize(DEFERRED_COUNT);

	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		gBuffers[i] = std::make_shared<RenderTargetView>();

		gBuffers[i]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

		_gBufferViews[i] = gBuffers[i]->GetRenderTargetView();	// rtv 가져온다.
	}

	//for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	//{
	//	gBuffers[i] = std::make_shared<RenderTargetView>();

	//	gBuffers[i]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32G32B32A32_FLOAT);

	//	_gBufferViews[i] = gBuffers[i]->GetRenderTargetView();	// rtv 가져온다.
	//}

	//gBuffers[DEFERRED_COUNT - 1] = std::make_shared<RenderTargetView>();

	//// !!! 얘는 DXGI_FORMAT_R32_UINT로 해줘야해 SV_Target을 uint로 뽑을거기때문에
	//gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight(), DXGI_FORMAT_R32_UINT);

	//_gBufferViews[DEFERRED_COUNT - 1] = gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_deferredDSV = std::make_shared<DepthStencilView>();

	_deferredDSV->Initialize(g_device, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());

	_screenViewPort = std::make_shared<ViewPort>();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());

	tempMat = std::make_shared<Material>();
	tempMat->metallic = 0.1f;
	tempMat->roughness = 0.0f;
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
	
	_model_PBR_VS.reset();
	_model_PBR_Skinned_VS.reset();
	_model_PBR_PS.reset();
	_skybox_VS.reset();
	_skybox_PS.reset();
	_mesh.reset();
	_mat.reset();
}

void DeferredPass::OnResize(int width, int height)
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		gBuffers[i]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_gBufferViews[i].ReleaseAndGetAddressOf();

		_gBufferViews[i] = gBuffers[i]->GetRenderTargetView();
	}

	//gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32_UINT);

	//_gBufferViews[DEFERRED_COUNT - 1].ReleaseAndGetAddressOf();
	//_gBufferViews[DEFERRED_COUNT - 1] = gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

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

void DeferredPass::Render(std::vector<std::shared_ptr<ObjectInfo>> objInfos, std::shared_ptr<DepthStencilView> shadowDSV)
{
	RenderStart();

	for (auto& objInfo : objInfos)
	{
		_mesh = ResourceManager::Get()->GetMesh(objInfo->meshID);

		switch (objInfo->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			cbPerObject cbPerObejctBuffer;
			cbPerObejctBuffer.gWorld = objInfo->worldTM;
			cbPerObejctBuffer.gWorldViewProj = objInfo->worldTM * RenderManager::s_cameraInfo->viewTM * RenderManager::s_cameraInfo->projTM;
			XMVECTOR det = XMMatrixDeterminant(objInfo->worldTM);
			cbPerObejctBuffer.gWorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, objInfo->worldTM));
			
			for (int i = 0; i < objInfo->materials.size(); i++)
			{
				_mat = ResourceManager::Get()->GetMaterial(objInfo->materials[i]);

				if (_mat == nullptr)
					_mat = tempMat;

				cbMaterial cbMaterialBuffer;
				cbMaterialBuffer.metallic = _mat->metallic;

				if(_mat->roughness >= 1.f)
					cbMaterialBuffer.roughness = 1.f;
				else
					cbMaterialBuffer.roughness = _mat->roughness;

				_model_PBR_VS->ConstantBufferUpdate(&cbMaterialBuffer, "cbMaterial");
				
				// Skinned Mesh
				if (objInfo->isSkinned)
				{
					memcpy(&cbPerObejctBuffer.gBoneTransforms, objInfo->finalBoneListMatrix, sizeof(Matrix) * 96);
					_model_PBR_Skinned_VS->ConstantBufferUpdate(&cbPerObejctBuffer, "cbPerObject");
					_model_PBR_Skinned_VS->Update();
				}
				// Static Mesh
				else
				{
					_model_PBR_VS->ConstantBufferUpdate(&cbPerObejctBuffer, "cbPerObject");
					_model_PBR_VS->Update();
				}

				cbMaterialBuffer.AddColor = Vector3(_mat->material_Diffuse.x, _mat->material_Diffuse.y, _mat->material_Diffuse.z);
				cbMaterialBuffer.emissiveColor = Vector3(_mat->material_Emissive.x, _mat->material_Emissive.y, _mat->material_Emissive.z);
				cbMaterialBuffer.emissionFactor = _mat->emissionFactor;

				_model_PBR_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::Get()->GetShader(objInfo->psName));

				// 일단..
				if (_mat->albedoMap != L"")
					_model_PBR_PS->SetResourceViewBuffer(_mat->albedoMap, "AlbedoMap");

				if (_mat->normalMap != L"")
					_model_PBR_PS->SetResourceViewBuffer(_mat->normalMap, "NormalMap");

				if (_mat->metallicMap != L"")
					_model_PBR_PS->SetResourceViewBuffer(_mat->metallicMap, "MetallicMap");

				if (_mat->roughnessMap != L"")
					_model_PBR_PS->SetResourceViewBuffer(_mat->roughnessMap, "RoughnessMap");

				if (_mat->AOMap != L"")
					_model_PBR_PS->SetResourceViewBuffer(_mat->AOMap, "AmbientOcclusionMap");

				if (_mat->emissiveMap != L"")
					_model_PBR_PS->SetResourceViewBuffer(_mat->emissiveMap, "EmissiveMap");
				
				_model_PBR_PS->ConstantBufferUpdate(&cbMaterialBuffer, "cbMaterial");

				_model_PBR_PS->Update();

				g_deviceContext->RSSetState(_mesh->GetRasterState().Get());

				unsigned int stride = _mesh->stride;
				unsigned int offset = 0;

				// 토폴로지 설정
				g_deviceContext->IASetPrimitiveTopology(_mesh->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(_mesh->indexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다
				g_deviceContext->DrawIndexed(_mesh->indexBuffersSize[i], 0, 0);
			}
		}
		break;
		case OBJECT_TYPE::SKY_BOX:
		{
			Graphics_Interface::Get()->TurnSkyboxOn();	// 스카이박스용 뎁스

			cbPerObject cbPerObejctBuffer;
			Matrix camPos = XMMatrixTranslationFromVector(RenderManager::s_cameraInfo->worldPos);
			cbPerObejctBuffer.gWorldViewProj = camPos * RenderManager::s_cameraInfo->viewTM * RenderManager::s_cameraInfo->projTM;
			cbPerObejctBuffer.gWorld = objInfo->worldTM;

			_skybox_VS->ConstantBufferUpdate(&cbPerObejctBuffer, "cbPerObject");
			_skybox_VS->Update();

			for (int i = 0; i < objInfo->materials.size(); i++)
			{
				_mat = ResourceManager::Get()->GetMaterial(objInfo->materials[i]);

				if (_mat == nullptr)
					_mat = tempMat;

				cbMaterial cbMaterialBuffer;
				cbMaterialBuffer.isLight = false;

				_skybox_PS->ConstantBufferUpdate(&cbMaterialBuffer, "cbMaterial");
				_skybox_PS->SetResourceViewBuffer(_mat->cubeMap, "CubeMap");
				_skybox_PS->Update();

				g_deviceContext->RSSetState(Graphics_Interface::Get()->GetSolidNoneCull()->GetrasterizerState().Get());

				unsigned int stride = _mesh->stride;
				unsigned int offset = 0;

				// 토폴로지 설정
				g_deviceContext->IASetPrimitiveTopology(_mesh->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(_mesh->indexBuffers[i].Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다
				g_deviceContext->DrawIndexed(_mesh->indexBuffersSize[i], 0, 0);
			}

			Graphics_Interface::Get()->TurnZBufferOn();	// 원래 뎁스로 돌려주기
		}
		break;
		default:
			break;
		}
	}
}

