#pragma once

class GameProcess
{
public:
	GameProcess();
	~GameProcess();

private:
	LPCWSTR _applicationName;
	HINSTANCE _hInstance;
	HWND _hWnd;

public:
	void InitializeWindows(int width, int height);
	void Run();
	void ShutdownWindows();

	void Init();
	void Update();
	void Shutdown();
	void RecalcWindowSize();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

static GameProcess* ApplicationHandle = 0;