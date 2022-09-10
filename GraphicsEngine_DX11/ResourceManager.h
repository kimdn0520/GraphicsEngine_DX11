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

private:
	std::wstring _texturePath;

	std::unordered_map<std::wstring, Texture*> _textures;

	std::unordered_map<std::string, Mesh*> _meshs;

public:
	void Initialize();

	void Release();

	Texture* GetTexture(const std::wstring& name);

	void CreateTexture(const std::wstring& name, const std::wstring& path);

	Mesh* GetMesh(std::string name);

	template <typename T>
	void CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, std::string name, int topology, int rasterizerState);
};

template<typename T>
inline void ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, std::string name, int topology, int rasterizerState)
{
	Mesh* mesh = new Mesh();

	if (topology == 0)
		mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == 1)
		mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	if (rasterizerState == 0)
		mesh->SetRenderState(Graphics_Interface::Get()->GetWireState());
	else if (rasterizerState == 1)
		mesh->SetRenderState(Graphics_Interface::Get()->GetSolidState());

	mesh->CreateVertexBuffer(vertices);
	mesh->CreateIndexBuffer(indices);
	mesh->SetIndexBufferSize(indices.size());
	mesh->stride = sizeof(T);

	_meshs.insert(make_pair(name, mesh));
}


