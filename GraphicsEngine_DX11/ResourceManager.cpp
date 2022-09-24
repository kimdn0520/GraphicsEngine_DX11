#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Mesh.h"

std::shared_ptr<ResourceManager> ResourceManager::resourceManager = nullptr;

size_t ResourceManager::meshID = 0;

std::shared_ptr<ResourceManager> ResourceManager::Get()
{
	if (resourceManager == nullptr)
		resourceManager = std::make_shared<ResourceManager>();

	return resourceManager;
}

void ResourceManager::Initialize()
{
	_texturePath = L"Data/Texture/";
}

void ResourceManager::Release()
{
	for (auto texture : _textures)
		texture.second->Release();

	_textures.clear();

	_meshs.clear();
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::wstring& name)
{
	auto findIt = _textures.find(name);

	if (findIt != _textures.end())
		return findIt->second;

	std::wstring path = _texturePath + name;

	CreateTexture(name, path);

	return _textures[name];
}

void ResourceManager::CreateTexture(const std::wstring& name, const std::wstring& path)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->Initialize(path);									// 텍스쳐 srv 생성

	_textures.insert(make_pair(name, texture));
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(size_t id)
{
	return _meshs[id];
}
