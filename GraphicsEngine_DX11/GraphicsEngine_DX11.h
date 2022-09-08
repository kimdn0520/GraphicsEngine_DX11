#pragma once
#include "Graphics_Interface.h"

namespace GraphicsEngine
{
	class GraphicsEngine_DX11 : public Graphics_Interface
	{
	public:
		GraphicsEngine_DX11() = default;
		virtual ~GraphicsEngine_DX11() = default;

	private:
		

	public:
		virtual void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

		virtual void Release() override;

		virtual void OnResize(const int& screenWidth, const int& screenHeight) override;

		// beginrenderbackbuffer -> 백버퍼 초기화
		 
		// endrender -> present
	};
}


