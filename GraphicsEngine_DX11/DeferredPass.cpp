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

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();	// rtv 가져온다.
	}

	_gBuffers[DEFERRED_COUNT - 1] = new RenderTargetView();

	// !!! 얘는 DXGI_FORMAT_R32_UINT로 해줘야해 SV_Target을 uint로 뽑을거기때문에
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

		_gBufferViews[i].ReleaseAndGetAddressOf();	// 오류 나려나?
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

	// G-Buffer들이 혹시 쉐이더 리소스로 박혀있을 수 있으니 Unbind
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
	//// 렌더링 타겟을 다시 백버퍼로 돌린다.
	//GraphicsEngineAPI::GetInstance()->SetBackBufferRenderTarget();

	//// z버퍼를 꺼준다.
	//GraphicsEngineAPI::GetInstance()->TurnZBufferOff();

	//// left ~ right : -1.0f ~ 1.0f 군요..!
	//// top ~ bottom : 1.0f ~ -1.0f 군요..!
	//_deferredWindow->Render(g_deviceContext, Vector4(-1.0f, 1.0f, 1.0f, -1.0f));	// 크기 얘로 조절

	//// Solid로 설정
	//g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());

	//// 버텍스 쉐이더 업데이트
	//_deferred_VS->Update();

	//// 픽셀 쉐이더에 SRV 셋팅 
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[0]->GetSRV().Get(), "Albedo");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[1]->GetSRV().Get(), "Depth");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[2]->GetSRV().Get(), "Normal");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[3]->GetSRV().Get(), "Position");
	//_deferred_PS->SetResourceViewBuffer(_gBuffers[4]->GetSRV().Get(), "MaterialID");		// 빛을 받을지 안받을지 // 이름을 바꿔준다.

	///*_deferred_PS->SetResourceViewBuffer(_gReflectionSkySRV, "ReflectionSkyTexture");
	//_deferred_PS->SetResourceViewBuffer(_gReflectionObjectSRV, "ReflectionObjectTexture");*/

	////_deferred_PS->SetResourceViewBuffer(ResourceManager::)

	//_deferred_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
	//_deferred_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

	//_cbTextureBuf.textureInfo = Vector4(												// 이녀석으로 텍스쳐의 uint를 뽑으려고 텍스쳐 사이즈 건네줌
	//	static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().width),
	//	static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().height),
	//	1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, 1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().height);
	//_deferred_PS->ConstantBufferUpdate(&_cbTextureBuf, "cbTexture");

	//_deferred_PS->Update();

	//// 그린다!
	//g_deviceContext->DrawIndexed(_deferredWindow->GetIndexCount(), 0, 0);

	//// z버퍼 다시 켜준다.
	//GraphicsEngineAPI::GetInstance()->TurnZBufferOn();
}
