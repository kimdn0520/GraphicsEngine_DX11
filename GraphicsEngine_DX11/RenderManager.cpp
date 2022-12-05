#include "pch.h"
#include "RenderManager.h"
#include "PassBase.h"
#include "ShadowPass.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "BloomPass.h"
#include "FinalPass.h"

std::shared_ptr<RenderManager> RenderManager::renderManager = nullptr;
std::shared_ptr<CameraInfo> RenderManager::s_cameraInfo;

std::shared_ptr<RenderManager> RenderManager::Get()
{
	if (renderManager == nullptr)
		renderManager = std::make_shared<RenderManager>();

	return renderManager;
}

void RenderManager::Initialize()
{
	isInit = true;

	PassBase::Initialize();

	_shadowPass = std::make_shared<ShadowPass>();
	_shadowPass->Start();

	_deferredPass = std::make_shared<DeferredPass>();
	_deferredPass->Start();

	_lightPass = std::make_shared<LightPass>();
	_lightPass->Start();

	_bloomPass = std::make_shared<BloomPass>();
	_bloomPass->Start();

	_finalPass = std::make_shared<FinalPass>();
	_finalPass->Start();
}

void RenderManager::OnResize(int width, int height)
{
	_shadowPass->OnResize(width, height);

	_deferredPass->OnResize(width, height);

	_lightPass->OnResize(width, height);

	_bloomPass->OnResize(width, height);
}

void RenderManager::Release()
{
	_renderData.clear();
	
	PassBase::Reset();

	_shadowPass->Release();

	_deferredPass->Release();

	_lightPass->Release();

	_bloomPass->Release();

	_finalPass->Release();
}

void RenderManager::PushRenderData(std::shared_ptr<ObjectInfo> objectInfo)
{
	_renderData.push_back(objectInfo);
}

void RenderManager::Render()
{
	// 섀도우
	_shadowPass->Render(_renderData);
	
	// 디퍼드 렌더
	_deferredPass->Render(_renderData, _shadowPass->shadowDSV);

	// Emissive Bloom
	_bloomPass->Render(_deferredPass->gBuffers[4]);

	// SSAO
	
	// 라이트 렌더 (디퍼드에서 모아놓은 텍스쳐에 빛 지연 연산)
	_lightPass->Render(_deferredPass->gBuffers, _shadowPass->shadowDSV, _bloomPass->bloomOutput, nullptr);

	// 포스트 프로세싱
	
	// UI

	// Debug

	// Final
	_finalPass->Render(_lightPass->lightingRTV);

	_renderData.clear();
}
