#pragma once
#include "GameEngineDLL.h"

extern "C" GameEngine_DLL void EngineInit(HWND hwnd, int width, int height);
extern "C" GameEngine_DLL void EngineUpdate();
extern "C" GameEngine_DLL void EngineRender();
extern "C" GameEngine_DLL void EngineShutdown();
extern "C" GameEngine_DLL void ResizeWindow(int width, int height);