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
	GameEngine_DLL Resources() {};
	~Resources() {};

private:
	static Resources* resources;

public:
	static GameEngine_DLL Resources* Get();

private:
	ASEParser* _aseParser;

	// unordered_map<string, shared_ptr<MeshData>> aseMeshData;

public:
	void Initialize();

	void Release();

	void LoadCubeMesh(int topology, int rasterState);

	void LoadSphereMesh(int topology, int rasterState);

	void LoadAxisMesh(int topology, int rasterState);

	void LoadGridMesh(int topology, int rasterState);

	void LoadScreenMesh(int topology, int rasterState);

	GameEngine_DLL std::vector<GameObject*> LoadASE(std::string path, int topology, int rasterizerState);

	// GameEngine_DLL MeshData* LoadFBX(const wstring& path, int topology, int rasterizerState, int pass);
};

