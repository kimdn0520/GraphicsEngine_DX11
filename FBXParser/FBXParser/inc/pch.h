// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define NOMINMAX						// windows에서 이미 정의 되어 있는 max 매크로와 min 매크로가 존재하기 때문에 이름이 충돌나는데 이걸 추가하면 해결!
#include <windows.h>
#include "fbxsdk.h"
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <tuple>
#include "SimpleMath.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

namespace fs = std::filesystem;

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\Library\\lib\\FBX\\debug\\libfbxsdk-md.lib")
#pragma comment(lib, "..\\..\\Library\\lib\\FBX\\debug\\libxml2-md.lib")
#pragma comment(lib, "..\\..\\Library\\lib\\FBX\\debug\\zlib-md.lib")
#else
#pragma comment(lib, "..\\..\\Library\\lib\\FBX\\release\libfbxsdk-md.lib")
#pragma comment(lib, "..\\..\\Library\\lib\\FBX\\release\libxml2-md.lib")
#pragma comment(lib, "..\\..\\Library\\lib\\FBX\\release\\zlib-md.lib")
#endif

#endif //PCH_H
