#pragma once
#include "Graphics_Interface.h"
#include "WindowInfo.h"

namespace GraphicsEngine
{
	class Device;
	class SwapChain;
	class DepthStencilState;
		
	class GraphicsEngine_DX11 : public Graphics_Interface
	{
	public:
		GraphicsEngine_DX11() = default;
		virtual ~GraphicsEngine_DX11() = default;

	private:
		WindowInfo _windowInfo;

		Device* _device;
		SwapChain* _swapChain;

		DepthStencilState* _depthStencilState;
		DepthStencilState* _disableDepthStencilState;
		DepthStencilState* _skyBoxDepthStencilState;

		

	public:
		virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

		virtual void Release() override;

		virtual void OnResize(const int& screenWidth, const int& screenHeight) override;

		// beginrenderbackbuffer -> 백버퍼 초기화
		 
		// endrender -> present
	};
}


