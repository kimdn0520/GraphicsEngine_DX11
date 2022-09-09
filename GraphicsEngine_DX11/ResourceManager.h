#pragma once
#include <string>

namespace GraphicsEngine
{
	class Texture;
	class Mesh;

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
	};
}

