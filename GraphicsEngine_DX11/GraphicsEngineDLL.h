#pragma once

#ifdef GraphicsEngine_EXPORTS
#define GraphicsEngine_DLL __declspec(dllexport)
#else
#define GraphicsEngine_DLL __declspec(dllimport)
#endif

