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

	std::shared_ptr<DepthStencilView> shadowDSV;

private:

	std::shared_ptr<ViewPort> _screenViewPort;

private:
	std::shared_ptr<VertexShader> _shadow_VS;
	std::shared_ptr<VertexShader> _shadow_Skinned_VS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(std::vector<std::shared_ptr<ObjectInfo>> meshs);

	void RenderEnd();
};

