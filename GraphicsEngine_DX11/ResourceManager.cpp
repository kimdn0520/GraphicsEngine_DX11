#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Mesh.h"

namespace GraphicsEngine
{
	ResourceManager* ResourceManager::resourceManager = nullptr;

	ResourceManager* ResourceManager::Get()
	{
		if (resourceManager == nullptr)
			resourceManager = new ResourceManager();

		return resourceManager;
	}

	void ResourceManager::Initialize()
	{
		_texturePath = L"Data/Texture/";
	}

	void ResourceManager::Release()
	{
		for(auto texture : _textures)
			texture.second->Release();
		
		for(auto mesh : _meshs)
			delete mesh.second;

		_textures.clear();
		
		_meshs.clear();
	}
	
	Texture* ResourceManager::GetTexture(const std::wstring& name)
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
		Texture* texture = new Texture();
		texture->Initialize(path);									// 텍스쳐 srv 생성

		_textures.insert(make_pair(name, texture));
	}
}