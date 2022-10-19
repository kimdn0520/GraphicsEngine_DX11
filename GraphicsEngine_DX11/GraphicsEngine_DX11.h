#pragma once
#include "Graphics_Interface.h"
#include "WindowInfo.h"

class Device;
class SwapChain;
class DepthStencilState;
class RasterizerState;
class RenderTargetView;
class DepthStencilView;
class BlendState;
class ViewPort;
		
class GraphicsEngine_DX11 : public Graphics_Interface
{
public:
	GraphicsEngine_DX11() = default;
	virtual ~GraphicsEngine_DX11() = default;

private:
	WindowInfo _windowInfo;

	std::shared_ptr<Device> _device;
	std::shared_ptr<SwapChain> _swapChain;

	std::shared_ptr<DepthStencilState> _depthStencilState;
	std::shared_ptr<DepthStencilState> _disableDepthStencilState;
	std::shared_ptr<DepthStencilState> _skyBoxDepthStencilState;

	std::shared_ptr<RasterizerState> _wireRasterizerState;
	std::shared_ptr<RasterizerState> _solidRasterizerState;
	std::shared_ptr<RasterizerState> _solidNoneCullRasterizerState;
	std::shared_ptr<RasterizerState> _shadowSolidRasterizerState;
	std::shared_ptr<RasterizerState> _shadowWireRasterizerState;

	std::shared_ptr<RenderTargetView> _mainRenderTargetView;

	std::shared_ptr<DepthStencilView> _depthStencilView;

	std::shared_ptr<BlendState> _alphaBlendState;

	std::shared_ptr<ViewPort> _mainViewPort;
		
public:
	virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	virtual void Release() override;

	virtual void OnResize(const int& screenWidth, const int& screenHeight) override;

	virtual void MainBackBufferRender() override;
		 
	virtual void RenderingDataRender() override;

	virtual void Present() override;

	virtual	int GetScreenWidth() override;
	
	virtual	int GetScreenHeight() override;
	
	virtual void RenderToBackBuffer() override;

	virtual void TurnZBufferOn() override;

	virtual void TurnZBufferOff() override;

	virtual void TurnSkyboxOn() override;

	virtual std::shared_ptr<Device> GetDeviceClass() override;

	virtual std::shared_ptr<RasterizerState> GetWire() override;

	virtual std::shared_ptr<RasterizerState> GetSolid() override;

	virtual std::shared_ptr<RasterizerState> GetSolidNoneCull() override;

	virtual std::shared_ptr<RasterizerState> GetShadowSolid() override;

	virtual std::shared_ptr<RasterizerState> GetShadowWire() override;

	virtual void SendObjectRenderingData(std::shared_ptr<ObjectInfo> objectInfo) override;

	virtual void SendCameraData(std::shared_ptr<CameraInfo> cameraInfo) override;

	virtual void SendLightData(std::shared_ptr<LightInfo> lightInfo) override;

	virtual void UpdateLightData(std::shared_ptr<LightInfo> lightInfo) override;

	virtual void SendMaterialData(std::shared_ptr<Material> material) override;

	virtual size_t CreateMesh(std::vector<StaticMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState) override;
	
	virtual size_t CreateMesh(std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indicies, int topology, int rasterState) override;

	virtual void MaterialReconstruction(std::string materialName, std::string textureName, std::string info) override;
};



