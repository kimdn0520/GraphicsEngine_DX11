#pragma once
#include "Mesh.h"

class Texture;

class ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager() {};

private:
	static ResourceManager* resourceManager;

public:
	static ResourceManager* Get();

public:
	static size_t meshID;

private:
	std::wstring _texturePath;

	std::unordered_map<std::wstring, Texture*> _textures;

	std::unordered_map<std::size_t, Mesh*> _meshs;

public:
	void Initialize();

	void Release();

	Texture* GetTexture(const std::wstring& name);

	void CreateTexture(const std::wstring& name, const std::wstring& path);

	Mesh* GetMesh(size_t id);

	template <typename T>
	size_t CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, int topology, int rasterizerState);
};

template<typename T>
inline size_t ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, int topology, int rasterizerState)
{
	Mesh* mesh = new Mesh();

	if (topology == 0)
		mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == 1)
		mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	mesh->rasterNum = rasterizerState;

	if (rasterizerState == 0)
		mesh->SetRenderState(Graphics_Interface::Get()->GetWire()->GetrasterizerState());
	else if (rasterizerState == 1)
		mesh->SetRenderState(Graphics_Interface::Get()->GetSolid()->GetrasterizerState());

	mesh->CreateVertexBuffer(vertices);
	mesh->CreateIndexBuffer(indices);
	mesh->SetIndexBufferSize(indices.size());
	mesh->stride = sizeof(T);
	
	mesh->meshID = meshID++;

	_meshs.insert(std::make_pair(mesh->meshID, mesh));

	return mesh->meshID;
}


