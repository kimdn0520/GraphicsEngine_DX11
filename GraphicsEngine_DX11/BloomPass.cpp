#include "pch.h"
#include "BloomPass.h"
#include "RenderTargetView.h"


void BloomPass::Render(std::shared_ptr<RenderTargetView> targetScreen)
{
	// BlumCurve�� Bloom�� �� ���� ã�´�.
	// targetScreen(emissive texture)�� bloomCurve_PS�� �Ἥ bloomSample�� �׷��ش�.
	// bloomSample�� �ٿ���ø� ���ش�.

	// �ٿ� ���ø�

	// Blur �� �ռ� �۾�..
}

void BloomPass::BloomCurveTarget(std::shared_ptr<RenderTargetView> targetScreen)
{
}
