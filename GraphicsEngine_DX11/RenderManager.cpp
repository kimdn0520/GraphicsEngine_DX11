#include "pch.h"
#include "RenderManager.h"

RenderManager* RenderManager::renderManager = nullptr;
CameraInfo RenderManager::s_cameraInfo;

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
	// pass ∏∂¥Ÿ¿« resize
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

}
