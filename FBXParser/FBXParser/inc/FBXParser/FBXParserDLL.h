#pragma once

#ifdef FBXParser_EXPORTS
#define FBXParser_DLL __declspec(dllexport)
#else
#define FBXParser_DLL __declspec(dllimport)
#endif