#pragma once
#include "GameEngineDLL.h"

class ASEParser;
class GameObject;

constexpr int RasterState_WIRE = 0;
constexpr int RasterState_SOLID = 1;
constexpr int TOPOLOGY_TRIANGLELIST = 0;
constexpr int TOPOLOGY_LINELIST = 1;

class Resources
{	
public:
	GameEngine_DLL Resources() = default;
	~Resources() = default;

private:
	static std::shared_ptr<Resources> resources;

public:
	static GameEngine_DLL std::shared_ptr<Resources> Get();

private:
	std::shared_ptr<ASEParser> _aseParser;

	// unordered_map<string, shared_ptr<MeshData>> aseMeshData;

public:
	void Initialize();

	void Release();

	void LoadCubeMesh(int topology, int rasterState);			// 0

	void LoadSphereMesh(int topology, int rasterState);			// 1

	void LoadAxisMesh(int topology, int rasterState);			// 2

	void LoadGridMesh(int topology, int rasterState);			// 3

	void LoadScreenMesh(int topology, int rasterState);			// 4

	void LoadDebugScreenMesh(int topology, int rasterState);	// 5

	GameEngine_DLL std::vector<std::shared_ptr<GameObject>> LoadASE(std::string path, int topology, int rasterizerState);

	std::vector<std::shared_ptr<GameObject>> LoadFBX(std::string path, int topology, int rasterizerState);
};

