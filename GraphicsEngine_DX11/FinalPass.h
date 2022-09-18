#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class ViewPort;

class FinalPass : public PassBase
{
public:
	FinalPass() = default;
	~FinalPass() = default;

private:
	ViewPort* _screenViewPort;

private:
	VertexShader* _quad_VS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render();

	void RenderEnd();
};

