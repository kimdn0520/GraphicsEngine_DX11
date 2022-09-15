#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class ViewPort;

// [0 : DEPTH], [1 : Normal], [2 : Position], [3 : Albedo], [4 : ObjectID] 
constexpr int DEFERRED_COUNT = 5;

/// <summary>
/// ������ ������ �� ó�� ���ִ� Pass �׷��� ���� ������(���۵� ������) �̶�� �Ѵ�.
/// </summary>
class DeferredPass : public PassBase
{
public:
	DeferredPass() = default;
	~DeferredPass() = default;

private:
	RenderTargetView* _gBuffers[DEFERRED_COUNT];

	ComPtr<ID3D11RenderTargetView> _gBufferViews[DEFERRED_COUNT];

	ViewPort* _screenViewPort;

private:
	VertexShader* _model_VS;
	VertexShader* _model_Skinned_VS;
	PixelShader* _model_PS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void BeginRender();

	void Render(std::vector<ObjectInfo*> meshs);

	void EndRender();
};

