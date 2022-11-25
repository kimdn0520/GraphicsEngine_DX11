#pragma once

class RenderTargetView;
class GaussianBlurPass;
class ViewPort;
class PixelShader;

/// <summary>
/// 1. Target�� �޾� Bloom Curve�� ���� Bloom�� �� ���� ã�´�.
/// 2. Donw16X�� Blur �� �� Down8X�� �ռ��Ѵ�.
/// 4. �ռ��� 8X�� Blur �� �� Down4X�� �ռ��Ѵ�.
/// 5. �ռ��� 4X�� Blur �� �� Down2X�� �ռ��Ѵ�.
/// 6. �ռ��� 2X�� Blur �� �� ������ �ռ��Ѵ�.
/// 7. ȭ�鿡 �ؽ��ĸ� �׸���.
/// </summary>
class BloomPass
{
private:
	std::shared_ptr<ViewPort> screenViewPort;

	std::shared_ptr<GaussianBlurPass> blurPass;

	std::shared_ptr<RenderTargetView> bloomSample;

	std::shared_ptr<PixelShader> bloomCurve_PS;

public:
	void Render(std::shared_ptr<RenderTargetView> targetScreen);

	void BloomCurveTarget(std::shared_ptr<RenderTargetView> targetScreen);


};

