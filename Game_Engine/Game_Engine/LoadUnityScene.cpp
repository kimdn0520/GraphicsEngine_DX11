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
			// m_Mesh 10202 = Cube, 10207 = Sphere
			YAMLBinaryData::MeshFilter meshFilter;

			YAML::Node MeshFilter = yaml->yamlNodeList[i]["MeshFilter"];

			YAML::Node m_Mesh = MeshFilter["m_Mesh"];

			int meshID = m_Mesh["fileID"].as<int>();

			switch (meshID)
			{
			case 10202:
			{
				meshFilter.meshName = "CubeMesh";
			}
			break;
			case 10207:
			{
				meshFilter.meshName = "SphereMesh";
			}
			break;
			default:
				break;
			}

			scene.gameObjects.back().meshFilter = meshFilter;
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
			YAMLBinaryData::BoxCollider boxCollider;

			YAML::Node BoxCollider = yaml->yamlNodeList[i]["BoxCollider"];

			boxCollider.isTrigger = BoxCollider["m_IsTrigger"].as<bool>();

			YAML::Node m_Size = BoxCollider["m_Size"];

			boxCollider.size.x = m_Size["x"].as<float>();
			boxCollider.size.y = m_Size["y"].as<float>();
			boxCollider.size.z = m_Size["z"].as<float>();

			YAML::Node m_Center = BoxCollider["m_Center"];

			boxCollider.center.x = m_Center["x"].as<float>();
			boxCollider.center.y = m_Center["y"].as<float>();
			boxCollider.center.z = m_Center["z"].as<float>();

			scene.gameObjects.back().boxCollider = boxCollider;
		}
		else if (yaml->yamlNodeList[i]["SphereCollider"])
		{

		}
		else if (yaml->yamlNodeList[i]["PrefabInstance"])
		{
			YAMLBinaryData::Prefab prefab;


		}
	}

	yaml.reset();
}
