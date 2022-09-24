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
	std::shared_ptr<ViewPort> _screenViewPort;

private:
	std::shared_ptr<VertexShader> _quad_VS;
	std::shared_ptr<PixelShader> _debug_PS;

	std::shared_ptr<DeferredPass> _deferredPass;

	std::shared_ptr<ShadowPass> _shadowPass;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void Render();
};

