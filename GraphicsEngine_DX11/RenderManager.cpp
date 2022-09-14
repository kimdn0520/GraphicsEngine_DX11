#include "pch.h"
#include "RenderManager.h"
#include "PassBase.h"

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
}

void RenderManager::OnResize(int width, int height)
{
	// pass ������ resize
}

void RenderManager::Release()
{
	for (auto it : _renderData)
		delete it;

	_renderData.clear();
	
	PassBase::Reset();
}

void RenderManager::PushRenderData(ObjectInfo* objectInfo)
{
	_renderData.push_back(objectInfo);
}

void RenderManager::Render()
{
	// ������
	// ���۵� ����
	// SSAO
	// ����
	// ����Ʈ ���μ���
	// UI
	// Debug..
}
