#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DepthStencilView;
class ViewPort;
class Mesh;
struct Material;

/// <summary>
/// 다운샘플링과 블러를 담당하는 패스
/// </summary>
class GaussianBlur : public PassBase
{
public:
	GaussianBlur() = default;
	~GaussianBlur() = default;

private:
	std::shared_ptr<ViewPort> _screenViewPort;

	std::shared_ptr<RenderTargetView> blurOutput;			// 밖으로 내보낼 블러 텍스쳐

	std::shared_ptr<RenderTargetView> blurX_Texture;		// 수평, 수직 블러가 그려진 렌더 텍스쳐

	std::shared_ptr<ViewPort> halfViewPort;

	std::shared_ptr<RenderTargetView> downHalfTexture;

	std::shared_ptr<ViewPort> quarterViewPort;

	std::shared_ptr<RenderTargetView> downQuarterTexture;

	std::shared_ptr<ViewPort> octarViewPort;

	std::shared_ptr<RenderTargetView> downOctarTexture;

	std::shared_ptr<ViewPort> sixteenViewPort;

	std::shared_ptr<RenderTargetView> downSixteenTexture;

	std::shared_ptr<VertexShader> quad_VS;

	std::shared_ptr<PixelShader> sampling_PS;

	std::shared_ptr<PixelShader> blurX_PS;

	std::shared_ptr<PixelShader> blurY_PS;

public:
	void Start() override;

	void Release() override;

	void OnResize(int width, int height) override;

	void RenderStart(std::shared_ptr<RenderTargetView>& screen);

	void Render(std::shared_ptr<RenderTargetView>& screen);

	void RenderEnd();

	void DownSampling2x(std::shared_ptr<RenderTargetView>& screen);

	void DownSampling4x(std::shared_ptr<RenderTargetView>& screen);

	void DownSampling8x(std::shared_ptr<RenderTargetView>& screen);

	void DownSampling16x(std::shared_ptr<RenderTargetView>& screen);

public:
	inline std::shared_ptr<RenderTargetView> GetRenderTarget() { return blurOutput; }
};

