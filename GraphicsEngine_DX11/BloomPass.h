#pragma once
#include "PassBase.h"

class GaussianBlurPass;
class RenderTargetView;
class ViewPort;
class PixelShader;
class VertexShader;

/// <summary>
/// 1. Target�� �޾� Bloom Curve�� ���� Bloom�� �� ���� ã�´�.
/// 2. Donw16X�� Blur �� �� Down8X�� �ռ��Ѵ�.
/// 4. �ռ��� 8X�� Blur �� �� Down4X�� �ռ��Ѵ�.
/// 5. �ռ��� 4X�� Blur �� �� Down2X�� �ռ��Ѵ�.
/// 6. �ռ��� 2X�� Blur �� �� ������ �ռ��Ѵ�.
/// 7. ȭ�鿡 �ؽ��ĸ� �׸���.
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

