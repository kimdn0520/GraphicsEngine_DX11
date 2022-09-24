#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class ViewPort;
class DebugPass;

class FinalPass : public PassBase
{
public:
	FinalPass() = default;
	~FinalPass() = default;

private:
	std::shared_ptr<ViewPort> _screenViewPort;

private:
	std::shared_ptr<VertexShader> _quad_VS;
	std::shared_ptr<PixelShader> _tone_PS;

	std::shared_ptr<DebugPass> _debugPass;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(std::shared_ptr<RenderTargetView> rtv);

	void RenderEnd();
};

