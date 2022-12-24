#include "pch.h"
#include "LoadUnityScene.h"

void LoadUnityScene::LoadScene(const std::string& path)
{
	shared_ptr<YAMLParser> yaml = make_shared<YAMLParser>();

	yaml->OpenFile(path);

	YAMLBinaryData::Scene scene;

	for (int i = 0; i < yaml->yamlNodeList.size(); i++)
	{
		if (yaml->yamlNodeList[i]["GameObject"])
		{
			YAMLBinaryData::GameObject gameObject;

			YAML::Node GameObject = yaml->yamlNodeList[i]["GameObject"];

			gameObject.name = GameObject["m_Name"].as<std::string>();

			scene.gameObjects.emplace_back(gameObject);
		}
		else if (yaml->yamlNodeList[i]["Transform"])
		{
			YAMLBinaryData::Transform transform;

			YAML::Node Transform = yaml->yamlNodeList[i]["Transform"]; 
			
			YAML::Node m_LocalPosition = Transform["m_LocalPosition"];

			transform.localPosition.x = m_LocalPosition["x"].as<float>();
			transform.localPosition.y = m_LocalPosition["y"].as<float>();
			transform.localPosition.z = m_LocalPosition["z"].as<float>();

			YAML::Node m_LocalRotation = Transform["m_LocalRotation"];

			transform.localRotation.x = m_LocalRotation["x"].as<float>();
			transform.localRotation.y = m_LocalRotation["y"].as<float>();
			transform.localRotation.z = m_LocalRotation["z"].as<float>();
			transform.localRotation.w = m_LocalRotation["w"].as<float>();

			YAML::Node m_LocalScale = Transform["m_LocalScale"];

			transform.localScale.x = m_LocalScale["x"].as<float>();
			transform.localScale.y = m_LocalScale["y"].as<float>();
			transform.localScale.z = m_LocalScale["z"].as<float>();

			scene.gameObjects.back().transform = transform;
		}
		else if (yaml->yamlNodeList[i]["MeshFilter"])
		{
			// m_Mesh 10202=Cube, 10207=Sphere
		}
		else if (yaml->yamlNodeList[i]["Camera"])
		{
			YAMLBinaryData::Camera camera;
		}
		else if (yaml->yamlNodeList[i]["Light"])
		{
			YAMLBinaryData::Light light;
		}
		else if (yaml->yamlNodeList[i]["BoxCollider"])
		{

		}
		else if (yaml->yamlNodeList[i]["SphereCollider"])
		{

		}
		else if (yaml->yamlNodeList[i]["PrefabInstance"])
		{

		}
	}

	yaml.reset();
}
