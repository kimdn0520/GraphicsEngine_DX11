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
	// pass ������ resize
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

void RenderManager::DeferredRender()
{
	_deferredPass->BeginRender();

	_deferredPass->Render(_renderData);

	_deferredPass->EndRender();
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
