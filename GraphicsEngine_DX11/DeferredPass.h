#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;

// [0 : DEPTH], [1 : Normal], [2 : Position], [3 : Albedo], [4 : ObjectID] 
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
	std::vector<RenderTargetView*> gBuffers;

private:
	ComPtr<ID3D11RenderTargetView> _gBufferViews[DEFERRED_COUNT];

	DepthStencilView* _deferredDSV;

	ViewPort* _screenViewPort;

private:
	VertexShader* _model_VS;
	VertexShader* _model_Skinned_VS;
	PixelShader* _model_PS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(std::vector<ObjectInfo*> meshs);

	void RenderEnd();
};

