#pragma once
#include "GraphicsEngineDLL.h"
#include "Graphics_RenderingData.h"
#include "Graphics_PerFrameData.h"
#include "VertexDefine.h"

class DepthStencilState;
class BlendState;
class RenderTargetView;
class Device;
class RasterizerState;
class WindowInfo;

class Graphics_Interface
{
public:
	Graphics_Interface() {};
	virtual ~Graphics_Interface() {};

private:
	static std::shared_ptr<Graphics_Interface> graphicsEngine;

public:
	static GraphicsEngine_DLL std::shared_ptr<Graphics_Interface> Get();

public:
	virtual GraphicsEngine_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;

	virtual GraphicsEngine_DLL void Release() abstract;

	virtual GraphicsEngine_DLL void OnResize(const int& screenWidth, const int& screenHeight) abstract;

	virtual GraphicsEngine_DLL void MainBackBufferRender() abstract;

	virtual GraphicsEngine_DLL void RenderingDataRender() abstract;

	virtual GraphicsEngine_DLL void Present() abstract;

	virtual	int GetScreenWidth() abstract;

	virtual	int GetScreenHeight() abstract;

	virtual void RenderToBackBuffer() abstract;

	virtual void TurnZBufferOn() abstract;

	virtual void TurnZBufferOff() abstract;

	virtual void TurnSkyboxOn() abstract;

	virtual GraphicsEngine_DLL std::shared_ptr<Device> GetDeviceClass() abstract;

	virtual GraphicsEngine_DLL std::shared_ptr<RasterizerState> GetWire() abstract;

	virtual GraphicsEngine_DLL std::shared_ptr<RasterizerState> GetSolid() abstract;

	virtual GraphicsEngine_DLL std::shared_ptr<RasterizerState> GetSolidNoneCull() abstract;

	virtual GraphicsEngine_DLL std::shared_ptr<RasterizerState> GetShadowSolid() abstract;

	virtual GraphicsEngine_DLL std::shared_ptr<RasterizerState> GetShadowWire() abstract;

	virtual GraphicsEngine_DLL void SendObjectRenderingData(std::shared_ptr<ObjectInfo> objectInfo) abstract;

	virtual GraphicsEngine_DLL void SendCameraData(std::shared_ptr<CameraInfo> cameraInfo) abstract;

	virtual GraphicsEngine_DLL void SendLightData(std::shared_ptr<LightInfo> lightInfo) abstract;

	virtual GraphicsEngine_DLL void UpdateLightData(std::shared_ptr<LightInfo> lightInfo) abstract;

	virtual GraphicsEngine_DLL size_t CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState) abstract;
	
	virtual GraphicsEngine_DLL size_t CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState) abstract;
};



