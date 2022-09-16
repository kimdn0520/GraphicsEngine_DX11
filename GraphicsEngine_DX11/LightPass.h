#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
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

private:
	RenderTargetView* _depth_RT;
	RenderTargetView* _normal_RT;
	RenderTargetView* _position_RT;
	RenderTargetView* _albedo_RT;
	RenderTargetView* _objectID_RT;

	ViewPort* _screenViewPort;

private:
	VertexShader* _light_VS;
	PixelShader* _light_PS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart();

	void Render(const std::vector<RenderTargetView*> gBuffers);

	void RenderEnd();
};

