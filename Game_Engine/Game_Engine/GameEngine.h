#pragma once

class GameEngine
{
public:
	void Init(HWND hwnd, int width, int height);

	void Update();
		
	void Render();
		
	void Release();
};



