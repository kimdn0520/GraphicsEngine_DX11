// GameClient.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRTDBG_MAP_ALLOC

#include "pch.h"
#include "framework.h"
#include "GameClient.h"
#include "GameProcess.h"
#include "crtdbg.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(11174);
	//_CrtSetBreakAlloc(324);
	//_CrtSetBreakAlloc(323);
	//_CrtSetBreakAlloc(322);
	//_CrtSetBreakAlloc(305);
	//_CrtSetBreakAlloc(293);


	GameProcess* gameProcess = new GameProcess();

	if (!gameProcess)
		return 0;

	gameProcess->Init();

	gameProcess->Run();

	gameProcess->Shutdown();

	//_CrtDumpMemoryLeaks();
	return 0;
}