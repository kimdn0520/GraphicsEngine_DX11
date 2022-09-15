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
	static Graphics_Interface* graphicsEngine;

public:
	static GraphicsEngine_DLL Graphics_Interface* Get();

public:
	virtual GraphicsEngine_DLL void Initialize(HWND hwnd, int screenWidth, int screenHeight) abstract;

	virtual GraphicsEngine_DLL void Release() abstract;

	virtual GraphicsEngine_DLL void OnResize(const int& screenWidth, const int& screenHeight) abstract;

	virtual GraphicsEngine_DLL void MainBackBufferRender() abstract;

	virtual GraphicsEngine_DLL void RenderingDataRender() abstract;

	virtual GraphicsEngine_DLL void Present() abstract;

	virtual	int GetScreenWidth() abstract;

	virtual	int GetScreenHeight() abstract;

	virtual GraphicsEngine_DLL Device* GetDeviceClass() abstract;
							   
	virtual GraphicsEngine_DLL RasterizerState* GetWire() abstract;
							   
	virtual GraphicsEngine_DLL RasterizerState* GetSolid() abstract;
							   
	virtual GraphicsEngine_DLL RasterizerState* GetSolidNoneCull() abstract;

	virtual GraphicsEngine_DLL void SendObjectRenderingData(ObjectInfo* objectInfo) abstract;

	virtual GraphicsEngine_DLL void SendCameraData(CameraInfo* cameraInfo) abstract;

	virtual GraphicsEngine_DLL void SendLightData(LightInfo* lightInfo) abstract;

	virtual GraphicsEngine_DLL void UpdateLightData(LightInfo* lightInfo) abstract;

	virtual GraphicsEngine_DLL size_t CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState) abstract;
	
	virtual GraphicsEngine_DLL size_t CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState) abstract;
};



