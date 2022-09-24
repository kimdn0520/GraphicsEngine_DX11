#pragma once
#include "Graphics_PerFrameData.h"
#include "Graphics_RenderingData.h"

class ShadowPass;
class DeferredPass;
class LightPass;
class FinalPass;

class RenderManager
{
public:
	RenderManager() {};
	~RenderManager() {};

private:
	static std::shared_ptr<RenderManager> renderManager;

public:
	static std::shared_ptr<RenderManager> Get();

	static std::shared_ptr<CameraInfo> s_cameraInfo;

private:
	std::shared_ptr<ShadowPass> _shadowPass;

	std::shared_ptr<DeferredPass> _deferredPass;

	std::shared_ptr<LightPass> _lightPass;

	std::shared_ptr<FinalPass> _finalPass;

	std::vector<std::shared_ptr<ObjectInfo>> _renderData;

public:
	bool isInit = false;

	bool isColDebugSwitch = false;

public:
	void Initialize();

	void OnResize(int width, int height);

	void Release();

	void PushRenderData(std::shared_ptr<ObjectInfo> objectInfo);
	
	void Render();

public:
	std::shared_ptr<DeferredPass> GetDeferredpass() { return _deferredPass; }

	std::shared_ptr<ShadowPass> GetShadowPass() { return _shadowPass; }
};


