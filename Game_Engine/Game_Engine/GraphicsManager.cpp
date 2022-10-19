#include "pch.h"
#include "GraphicsManager.h"

std::shared_ptr<GraphicsManager> GraphicsManager::graphicsManager = nullptr;

std::shared_ptr<GraphicsManager> GraphicsManager::Get()
{
	if (graphicsManager == nullptr)
		graphicsManager = std::make_shared<GraphicsManager>();

	return graphicsManager;
}

void GraphicsManager::Initialize(HWND hWnd, int screenWidth, int screenHeight)
{
	// 그래픽스 엔진을 얻어온다.
	graphicsEngine = Graphics_Interface::Get();

	// 그래픽스 엔진을 Init 해준다.
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

void GraphicsManager::SendObjectRenderingData(std::shared_ptr<ObjectInfo> objectInfo)
{
	graphicsEngine->SendObjectRenderingData(objectInfo);
}

void GraphicsManager::SendCameraData(std::shared_ptr<CameraInfo> cameraInfo)
{
	graphicsEngine->SendCameraData(cameraInfo);
}

void GraphicsManager::SendLightData(std::shared_ptr<LightInfo> lightInfo)
{
	graphicsEngine->SendLightData(lightInfo);
}

void GraphicsManager::UpdateLightData(std::shared_ptr<LightInfo> lightInfo)
{
	graphicsEngine->UpdateLightData(lightInfo);
}

void GraphicsManager::SendMaterialData(std::shared_ptr<Material> material)
{
	graphicsEngine->SendMaterialData(material);
}

size_t GraphicsManager::CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState)
{
	return graphicsEngine->CreateMesh(vertices, indicies, topology, rasterState);
}

size_t GraphicsManager::CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState)
{
	return graphicsEngine->CreateMesh(vertices, indicies, topology, rasterState);
}

void GraphicsManager::MaterialReconstruction(std::string materialName, std::string textureName, std::string info)
{
	graphicsEngine->MaterialReconstruction(materialName, textureName, info);
}
