#pragma once
#include "Graphics_PerFrameData.h"
#include "Graphics_RenderingData.h"

class RenderManager
{
public:
	RenderManager() {};
	~RenderManager() {};

private:
	static RenderManager* renderManager;

public:
	static RenderManager* Get();

	static CameraInfo s_cameraInfo;

private:
	// pass

	std::vector<ObjectInfo*> _renderData;

public:
	bool isInit = false;

	bool isColDebugSwitch = false;

public:
	void Initialize();

	void OnResize(int width, int height);

	void Release();

	void PushRenderData(ObjectInfo* objectInfo);

	void Render();
};


