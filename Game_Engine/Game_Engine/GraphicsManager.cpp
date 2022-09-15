#include "pch.h"
#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::graphicsManager = nullptr;

GraphicsManager* GraphicsManager::Get()
{
	if (graphicsManager == nullptr)
		graphicsManager = new GraphicsManager();

	return graphicsManager;
}

void GraphicsManager::Initialize(HWND hWnd, int screenWidth, int screenHeight)
{
	// �׷��Ƚ� ������ ���´�.
	graphicsEngine = Graphics_Interface::Get();

	// �׷��Ƚ� ������ Init ���ش�.
	graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);
}

void GraphicsManager::Release()
{
	graphicsEngine->Release();
}

void GraphicsManager::OnResize(int width, int height)
{
	graphicsEngine->OnResize(width, height);
}

void GraphicsManager::MainBackBufferRender()
{
	graphicsEngine->MainBackBufferRender();
}

void GraphicsManager::RenderingDataRender()
{
	graphicsEngine->RenderingDataRender();
}

void GraphicsManager::Present()
{
	graphicsEngine->Present();
}

int GraphicsManager::GetScreenWidth()
{
	return graphicsEngine->GetScreenWidth();
}

int GraphicsManager::GetScreenHeight()
{
	return graphicsEngine->GetScreenHeight();
}

void GraphicsManager::SendObjectRenderingData(ObjectInfo* objectInfo)
{
	graphicsEngine->SendObjectRenderingData(objectInfo);
}

void GraphicsManager::SendCameraData(CameraInfo* cameraInfo)
{
	graphicsEngine->SendCameraData(cameraInfo);
}

void GraphicsManager::SendLightData(LightInfo* lightInfo)
{
	graphicsEngine->SendLightData(lightInfo);
}

void GraphicsManager::UpdateLightData(LightInfo* lightInfo)
{
	graphicsEngine->UpdateLightData(lightInfo);
}

size_t GraphicsManager::CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState)
{
	return graphicsEngine->CreateMesh(vertices, indicies, topology, rasterState);
}

size_t GraphicsManager::CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState)
{
	return graphicsEngine->CreateMesh(vertices, indicies, topology, rasterState);
}