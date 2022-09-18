#include "pch.h"
#include "ShadowPass.h"
#include "Graphics_Interface.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ViewPort.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RenderManager.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "LightManager.h"

void ShadowPass::Start()
{
	_shadow_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Shadow_VS"));
	_shadow_Skinned_VS = dynamic_cast<VertexShader*>(ShaderManager::Get()->GetShader(L"Shadow_Skinned_VS"));

	_screenViewPort = new ViewPort();

	_screenViewPort->Initialize(Vector2::Zero, Graphics_Interface::Get()->GetScreenWidth(), Graphics_Interface::Get()->GetScreenHeight());
}

void ShadowPass::Release()
{
}

void ShadowPass::OnResize(int width, int height)
{
}

void ShadowPass::RenderStart()
{
}

void ShadowPass::Render(const std::vector<RenderTargetView*> gBuffers)
{
}

void ShadowPass::RenderEnd()
{
}
