#pragma once
#include "Mesh.h"

class Texture;

class ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager() {};

private:
	static std::shared_ptr<ResourceManager> resourceManager;

public:
	static std::shared_ptr<ResourceManager> Get();

public:
	static size_t meshID;

private:
	std::wstring _texturePath;

	std::unordered_map<std::wstring, std::shared_ptr<Texture>> _textures;

	std::unordered_map<std::size_t, std::shared_ptr<Mesh>> _meshs;

	std::unordered_map<std::string, std::shared_ptr<Material>> _materials;

public:
	void Initialize();

	void Release();

	std::shared_ptr<Texture> GetTexture(const std::wstring& name);

	void CreateTexture(const std::wstring& name, const std::wstring& path);

	std::shared_ptr<Mesh>& GetMesh(size_t id);

	std::shared_ptr<Material>& GetMaterial(std::string name);

	void AddMaterial(std::shared_ptr<Material> material);

	template <typename T>
	size_t CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, int topology, int rasterizerState);
};

template<typename T>
inline size_t ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<unsigned int> indices, int topology, int rasterizerState)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

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


