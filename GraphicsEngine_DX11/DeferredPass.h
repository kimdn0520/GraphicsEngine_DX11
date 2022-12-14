#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;
class Mesh;
struct Material;

// [0 : DEPTH], [1 : Normal], [2 : Position], [3 : Albedo], [4 : ObjectID] 
// [0 : DMRAO], [1 : Normal], [2 : Position], [3 : Albedo], [4 : Emissive] 
constexpr int DEFERRED_COUNT = 5;

/// <summary>
/// 라이팅 연산을 후 처리 해주는 Pass 그래서 지연 렌더링(디퍼드 렌더링) 이라고 한다.
/// </summary>
class DeferredPass : public PassBase
{
public:
	DeferredPass() = default;
	~DeferredPass() = default;

public:
	std::vector<std::shared_ptr<RenderTargetView>> gBuffers;

private:
	ComPtr<ID3D11RenderTargetView> _gBufferViews[DEFERRED_COUNT];

	std::shared_ptr<DepthStencilView> _deferredDSV;

	std::shared_ptr<ViewPort> _screenViewPort;

private:
	std::shared_ptr<VertexShader> _model_PBR_VS;
	std::shared_ptr<VertexShader> _model_PBR_Skinned_VS;

	std::shared_ptr<PixelShader> _model_PBR_PS;

	std::shared_ptr<VertexShader> _skybox_VS;
	std::shared_ptr<PixelShader> _skybox_PS;

	std::shared_ptr<Mesh> _mesh;

	std::shared_ptr<Material> _mat;

	std::shared_ptr<Material> tempMat;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(std::vector<std::shared_ptr<ObjectInfo>> objInfos, std::shared_ptr<DepthStencilView> shadowDSV);
};

