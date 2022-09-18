#include "pch.h"
#include "RenderManager.h"
#include "PassBase.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "FinalPass.h"

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

	_lightPass = new LightPass();
	_lightPass->Start();

	_finalPass = new FinalPass();
	_finalPass->Start();
}

void RenderManager::OnResize(int width, int height)
{
	_deferredPass->OnResize(width, height);

	_lightPass->OnResize(width, height);
}

void RenderManager::Release()
{
	for (auto it : _renderData)
		delete it;

	_renderData.clear();
	
	PassBase::Reset();

	_deferredPass->Release();

	_lightPass->Release();
}

void RenderManager::PushRenderData(ObjectInfo* objectInfo)
{
	_renderData.push_back(objectInfo);
}

void RenderManager::Render()
{
	// ������
	
	// ���۵� ����
	_deferredPass->Render(_renderData);

	// SSAO
	
	// ����Ʈ ���� (���۵忡�� ��Ƴ��� �ؽ��Ŀ� �� ���� ����)
	_lightPass->Render(_deferredPass->gBuffers);

	// ����Ʈ ���μ���
	
	// UI

	// Debug

	// Final
	_finalPass->Render();

	_renderData.clear();
}
