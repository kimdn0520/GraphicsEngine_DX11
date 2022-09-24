#pragma once
#include "Graphics_Interface.h"

class GraphicsManager
{
public:
	GraphicsManager() {};

	~GraphicsManager() {};

private:
	static std::shared_ptr<GraphicsManager> graphicsManager;

private:
	std::shared_ptr<Graphics_Interface> graphicsEngine;
	
public:
	static std::shared_ptr<GraphicsManager> Get();

public:
	void Initialize(HWND hWnd, int screenWidth, int screenHeight);

	void Release();

	void OnResize(int width, int height);

	void MainBackBufferRender();

	void RenderingDataRender();

	void Present();

	int GetScreenWidth();

	int GetScreenHeight();

	void SendObjectRenderingData(std::shared_ptr<ObjectInfo> objectInfo);

	void SendCameraData(std::shared_ptr<CameraInfo> cameraInfo);

	void SendLightData(std::shared_ptr<LightInfo> lightInfo);

	void UpdateLightData(std::shared_ptr<LightInfo> lightInfo);

	size_t CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState);

	size_t CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState);
};


