#pragma once
#include "Graphics_Interface.h"

class GraphicsManager
{
public:
	GraphicsManager() {};

	~GraphicsManager() {};

private:
	static GraphicsManager* graphicsManager;

private:
	Graphics_Interface* graphicsEngine;
	
public:
	static GraphicsManager* Get();

public:
	void Initialize(HWND hWnd, int screenWidth, int screenHeight);

	void Release();

	void OnResize(int width, int height);

	void MainBackBufferRender();

	void RenderingDataRender();

	void Present();

	int GetScreenWidth();

	int GetScreenHeight();

	void SendObjectRenderingData(ObjectInfo* objectInfo);

	void SendCameraData(CameraInfo* cameraInfo);

	void SendLightData(LightInfo* lightInfo);

	void UpdateLightData(LightInfo* lightInfo);

	size_t CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState);

	size_t CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState);
};

