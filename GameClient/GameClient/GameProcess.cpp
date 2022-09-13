#include "pch.h"
#include "GameProcess.h"
#include "GameEngine_Interface.h"
#include "TestScene.h"

#include "SceneManager.h"
#include "DataManager.h"

const int screenWidth = 1920;
const int screenHeight = 1080;
const bool FULL_SCREEN = false;

GameProcess::GameProcess()
{}

GameProcess::~GameProcess()
{}

void GameProcess::InitializeWindows(int width, int height)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX = 0, posY = 0;

	// 이 개체에 대한 외부 포인터를 가져옵니다.
	ApplicationHandle = this;

	// 이 애플리케이션의 인스턴스를 가져옵니다.
	_hInstance = GetModuleHandle(NULL);

	// 애플리케이션 이름 설정
	_applicationName = L"JWEngine";

	// 기본 설정으로 Windows 클래스를 설정함.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 클래스 등록
	RegisterClassEx(&wc);

	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)GetSystemMetrics(SM_CXSCREEN);
		dmScreenSettings.dmPelsHeight = (unsigned long)GetSystemMetrics(SM_CYSCREEN);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Change the display settings to full screen. ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN); // Set the position of the window to the top left corner. posX = posY = 0; } else { // If windowed then set it to 800x600 resolution. screenWidth = 800; screenHeight = 600; // Place the window in the middle of the screen. posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2; posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2; }
	}
	else
	{
		// Place the window in the middle of the screen. 
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// 화면 설정으로 창을 만들고 핸들을 가져옵니다.
	_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, _applicationName, _applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY,
		width, height,
		NULL, NULL, _hInstance, NULL
	);

	// 창을 화면에 띄우고 메인 포커스로 설정합니다.
	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);

	// 윈도우 창 재설정
	RecalcWindowSize();

	// 마우스 커서를 숨깁니다.
	//ShowCursor(false); 

	return;
}

void GameProcess::Run()
{
	MSG msg;

	// 메시지 구조 초기화 
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			EngineUpdate();

			EngineRender();
		}
	}
}

void GameProcess::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode. 
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window. 
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	// Remove the application instance. 
	UnregisterClass(_applicationName, _hInstance);
	_hInstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

void GameProcess::Init()
{
	InitializeWindows(screenWidth, screenHeight);

	EngineInit(_hWnd, screenWidth, screenHeight);

	{
		SceneManager::Get()->InputScene<TestScene>("TestScene");
	
		SceneManager::Get()->LoadScene("TestScene");			
	}
}

void GameProcess::Shutdown()
{
	EngineShutdown();

	DataManager::Get()->Release();

	ShutdownWindows();
}

void GameProcess::RecalcWindowSize()
{
	RECT nowRect;
	DWORD _style = (DWORD)GetWindowLong(_hWnd, GWL_STYLE);
	DWORD _exstyle = (DWORD)GetWindowLong(_hWnd, GWL_EXSTYLE);

	GetWindowRect(_hWnd, &nowRect);

	RECT newRect;
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = screenWidth;
	newRect.bottom = screenHeight;

	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	// 클라이언트 영역보다 윈도 크기는 더 커야 한다. (외곽선, 타이틀 등)
	int _newWidth = (newRect.right - newRect.left);
	int _newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(_hWnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
		_newWidth, _newHeight, SWP_SHOWWINDOW);
}

LRESULT GameProcess::MessageHandler(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 다른 모든 메시지는 애플리케이션에서 사용하지 않기 때문에 기본 메시지 핸들러로 전송됩니다.
		default:
		{
			return DefWindowProc(hWnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK GameProcess::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_SIZE:
		{
			if (wparam != SIZE_MINIMIZED)
			{
				ResizeWindow(LOWORD(lparam), HIWORD(lparam));
			}

			return 0;
		}
		// Check if the window is being destroyed. 
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed. 
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// All other messages pass to the message handler in the system class. 
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}