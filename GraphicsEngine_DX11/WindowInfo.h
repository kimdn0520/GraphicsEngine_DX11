#pragma once

namespace GraphicsEngine
{
	struct WindowInfo
	{
		HWND hwnd;
		int screenWidth;
		int screenHeight;

		bool enable4xMSAA;
		unsigned int MSAAQuality_4x;

		bool fullScreen;
	};
}