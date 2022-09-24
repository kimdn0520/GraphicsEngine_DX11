#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;

/// <summary>
/// 라이트 연산을 해주는 Pass 
/// 디퍼드렌더에서 사용한 텍스쳐들을 받아와 빛을 후처리 연산 해준다.
/// </summary>
class LightPass : public PassBase
{
public:
	LightPass() = default;
	~LightPass() = default;

public:
	std::shared_ptr<RenderTargetView> lightingRTV;

private:
	std::shared_ptr<ViewPort> _screenViewPort;

private:
	std::shared_ptr<VertexShader> _quad_VS;
	std::shared_ptr<PixelShader> _light_PS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(const std::vector<std::shared_ptr<RenderTargetView>> gBuffers, std::shared_ptr<DepthStencilView> shadowDSV);

	void RenderEnd();
};

