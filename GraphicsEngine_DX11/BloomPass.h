#pragma once
#include "PassBase.h"

class GaussianBlurPass;
class RenderTargetView;
class ViewPort;
class PixelShader;
class VertexShader;

/// <summary>
/// 1. Target을 받아 Bloom Curve를 통해 Bloom이 될 곳을 찾는다.
/// 2. Donw16X를 Blur 한 후 Down8X에 합성한다.
/// 4. 합성된 8X를 Blur 한 후 Down4X에 합성한다.
/// 5. 합성된 4X를 Blur 한 후 Down2X에 합성한다.
/// 6. 합성된 2X를 Blur 한 후 원본과 합성한다.
/// 7. 화면에 텍스쳐를 그린다.
/// </summary>
class BloomPass : public PassBase
{
public:
	BloomPass() = default;
	~BloomPass() = default;

private:
	std::shared_ptr<ViewPort> screenViewPort;

	std::shared_ptr<GaussianBlurPass> blurPass;

	std::shared_ptr<RenderTargetView> bloomSample;

	std::shared_ptr<RenderTargetView> bloomOutput;

	std::shared_ptr<VertexShader> quad_VS;

	std::shared_ptr<PixelShader> bloomCurve_PS;

	std::shared_ptr<PixelShader> bloomAddUp_PS;

	std::shared_ptr<ViewPort> octarViewPort;

	std::shared_ptr<RenderTargetView> octarTarget;

	std::shared_ptr<ViewPort> quarterViewPort;

	std::shared_ptr<RenderTargetView> quarterTarget;

	std::shared_ptr<ViewPort> halfViewPort;

	std::shared_ptr<RenderTargetView> halfTarget;

	friend class RenderManager;

	friend class DebugPass;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void Render(std::shared_ptr<RenderTargetView> targetScreen);

	void RenderEnd();

	void BloomCurveTarget(std::shared_ptr<RenderTargetView> targetScreen);

	void Blur16XAddUp8X();

	void Blur8XAddUp4X();

	void Blur4XAddUp2X();

	void Blur2XAddUpOrigin(std::shared_ptr<RenderTargetView> targetScreen);
};

