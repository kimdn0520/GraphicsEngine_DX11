#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;
class DeferredPass;
class ShadowPass;

/// <summary>
/// ������ ������ �� ó�� ���ִ� Pass �׷��� ���� ������(���۵� ������) �̶�� �Ѵ�.
/// </summary>
class DebugPass : public PassBase
{
public:
	DebugPass() = default;
	~DebugPass() = default;

private:
	ViewPort* _screenViewPort;

private:
	VertexShader* _quad_VS;
	PixelShader* _debug_PS;

	DeferredPass* _deferredPass;

	ShadowPass* _shadowPass;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void Render();
};

