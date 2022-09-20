#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;

/// <summary>
/// 라이트 연산을 해주는 Pass 
/// 디퍼드렌더에서 사용한 텍스쳐들을 받아와 빛을 후처리 연산 해준다.
/// </summary>
class LightPass : public PassBase
{
public:
	LightPass() = default;
	~LightPass() = default;

public:
	RenderTargetView* lightingRTV;

private:
	ViewPort* _screenViewPort;

private:
	VertexShader* _quad_VS;
	PixelShader* _light_PS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(const std::vector<RenderTargetView*> gBuffers, DepthStencilView* shadowDSV);

	void RenderEnd();
};

