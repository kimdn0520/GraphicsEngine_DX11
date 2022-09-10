#pragma once

#ifdef GameEngine_EXPORTS
#define GameEngine_DLL __declspec(dllexport)
#else
#define GameEngine_DLL __declspec(dllimport)
#endif