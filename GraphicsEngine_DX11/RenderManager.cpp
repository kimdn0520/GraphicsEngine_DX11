#include "pch.h"
#include "RenderManager.h"

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
}

void RenderManager::PushRenderData(ObjectInfo* objectInfo)
{
	_renderData.push_back(objectInfo);
}

void RenderManager::Render()
{
	// 섀도우
	// 디퍼드 렌더
	// SSAO
	// 알파
	// 포스트 프로세싱
	// UI
	// Debug..
}
