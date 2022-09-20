#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;

/// <summary>
/// ����Ʈ ������ ���ִ� Pass 
/// ���۵巻������ ����� �ؽ��ĵ��� �޾ƿ� ���� ��ó�� ���� ���ش�.
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

