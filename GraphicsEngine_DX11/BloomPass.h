#pragma once

class RenderTargetView;
class GaussianBlurPass;
class ViewPort;
class PixelShader;

/// <summary>
/// 1. Target을 받아 Bloom Curve를 통해 Bloom이 될 곳을 찾는다.
/// 2. Donw16X를 Blur 한 후 Down8X에 합성한다.
/// 4. 합성된 8X를 Blur 한 후 Down4X에 합성한다.
/// 5. 합성된 4X를 Blur 한 후 Down2X에 합성한다.
/// 6. 합성된 2X를 Blur 한 후 원본과 합성한다.
/// 7. 화면에 텍스쳐를 그린다.
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

