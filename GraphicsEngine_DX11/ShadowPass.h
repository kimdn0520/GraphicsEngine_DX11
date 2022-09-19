#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;

class ShadowPass : public PassBase
{
public:
	ShadowPass() = default;
	~ShadowPass() = default;

private:
	DepthStencilView* _shadowDSV;

	ViewPort* _screenViewPort;

private:
	VertexShader* _shadow_VS;
	VertexShader* _shadow_Skinned_VS;

	cbLightViewProj cbLightViewProjBuffer;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(std::vector<ObjectInfo*> meshs);

	void RenderEnd();
};

