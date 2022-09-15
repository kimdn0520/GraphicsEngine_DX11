#pragma once
#include "Graphics_PerFrameData.h"
#include "Graphics_RenderingData.h"

class DeferredPass;

class RenderManager
{
public:
	RenderManager() {};
	~RenderManager() {};

private:
	static RenderManager* renderManager;

public:
	static RenderManager* Get();

	static CameraInfo* s_cameraInfo;

private:
	DeferredPass* _deferredPass;

	std::vector<ObjectInfo*> _renderData;

public:
	bool isInit = false;

	bool isColDebugSwitch = false;

public:
	void Initialize();

	void OnResize(int width, int height);

	void Release();

	void PushRenderData(ObjectInfo* objectInfo);
	
	void DeferredRender();

	void Render();
};


