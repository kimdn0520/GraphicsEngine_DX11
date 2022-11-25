#include "pch.h"
#include "BloomPass.h"
#include "RenderTargetView.h"


void BloomPass::Render(std::shared_ptr<RenderTargetView> targetScreen)
{
	// BlumCurve로 Bloom이 될 곳을 찾는다.
	// targetScreen(emissive texture)를 bloomCurve_PS를 써서 bloomSample에 그려준다.
	// bloomSample을 다운샘플링 해준다.

	// 다운 샘플링

	// Blur 및 합성 작업..
}

void BloomPass::BloomCurveTarget(std::shared_ptr<RenderTargetView> targetScreen)
{
}
