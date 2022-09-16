#include "pch.h"
#include "RenderManager.h"
#include "PassBase.h"
#include "DeferredPass.h"

RenderManager* RenderManager::renderManager = nullptr;
CameraInfo* RenderManager::s_cameraInfo;

RenderManager* RenderManager::Get()
{
	if (renderManager == nullptr)
		renderManager = new RenderManager();

	return renderManager;
}

void RenderManager::Initialize()
{
	isInit = true;

	PassBase::Initialize();

	_deferredPass = new DeferredPass();
	_deferredPass->Start();
}

void RenderManager::OnResize(int width, int height)
{
	// pass 마다의 resize
}

void RenderManager::Release()
{
	for (auto it : _renderData)
		delete it;

	_renderData.clear();
	
	PassBase::Reset();

	_deferredPass->Release();
}

void RenderManager::PushRenderData(ObjectInfo* objectInfo)
{
	_renderData.push_back(objectInfo);
}

void RenderManager::Render()
{
	// 섀도우
	
	// 디퍼드 렌더
	//_deferredPass->Render(_renderData);

	// SSAO
	
	// 라이트 렌더 (디퍼드에서 모아놓은 텍스쳐에 빛 지연 연산)

	// 포스트 프로세싱
	
	// UI
	// Debug

	// EndRender 는 따로 만들까?

	_renderData.clear();
}
