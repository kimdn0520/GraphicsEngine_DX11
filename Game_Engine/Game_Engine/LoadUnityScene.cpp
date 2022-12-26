#include "pch.h"
#include "LoadUnityScene.h"

void LoadUnityScene::LoadScene(const std::string& path)
{
	shared_ptr<YAMLParser> yaml = std::make_shared<YAMLParser>();

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
			std::shared_ptr<YAMLBinaryData::Transform> transform = std::make_shared<YAMLBinaryData::Transform>();

			YAML::Node Transform = yaml->yamlNodeList[i]["Transform"]; 
			
			YAML::Node m_LocalPosition = Transform["m_LocalPosition"];

			transform->localPosition.x = m_LocalPosition["x"].as<float>();
			transform->localPosition.y = m_LocalPosition["y"].as<float>();
			transform->localPosition.z = m_LocalPosition["z"].as<float>();

			YAML::Node m_LocalEulerAnglesHint = Transform["m_LocalEulerAnglesHint"];

			transform->localRotation.x = m_LocalEulerAnglesHint["x"].as<float>();
			transform->localRotation.y = m_LocalEulerAnglesHint["y"].as<float>();
			transform->localRotation.z = m_LocalEulerAnglesHint["z"].as<float>();

			YAML::Node m_LocalScale = Transform["m_LocalScale"];

			transform->localScale.x = m_LocalScale["x"].as<float>();
			transform->localScale.y = m_LocalScale["y"].as<float>();
			transform->localScale.z = m_LocalScale["z"].as<float>();

			scene.gameObjects.back().transform = transform;
		}
		else if (yaml->yamlNodeList[i]["MeshFilter"])
		{
			// m_Mesh 10202 = Cube, 10207 = Sphere
			std::shared_ptr<YAMLBinaryData::MeshFilter> meshFilter = std::make_shared<YAMLBinaryData::MeshFilter>();

			YAML::Node MeshFilter = yaml->yamlNodeList[i]["MeshFilter"];

			YAML::Node m_Mesh = MeshFilter["m_Mesh"];

			int meshID = m_Mesh["fileID"].as<int>();

			switch (meshID)
			{
			case 10202:
			{
				meshFilter->meshName = "CubeMesh";
			}
			break;
			case 10207:
			{
				meshFilter->meshName = "SphereMesh";
			}
			break;
			case 10209:
			{
				meshFilter->meshName = "PlaneMesh";
			}
			break;
			default:
				break;
			}

			scene.gameObjects.back().meshFilter = meshFilter;
		}
		else if (yaml->yamlNodeList[i]["Camera"])
		{
			std::shared_ptr<YAMLBinaryData::Camera> camera = std::make_shared<YAMLBinaryData::Camera>();
			
			YAML::Node Camera = yaml->yamlNodeList[i]["Camera"];

			camera->nearPlane = Camera["near clip plane"].as<float>();

			camera->farPlane = Camera["far clip plane"].as<float>();

			scene.gameObjects.back().camera = camera;
		}
		else if (yaml->yamlNodeList[i]["Light"])
		{
			std::shared_ptr<YAMLBinaryData::Light> light = std::make_shared<YAMLBinaryData::Light>();
		}
		else if (yaml->yamlNodeList[i]["BoxCollider"])
		{
			std::shared_ptr<YAMLBinaryData::BoxCollider> boxCollider = std::make_shared<YAMLBinaryData::BoxCollider>();

			YAML::Node BoxCollider = yaml->yamlNodeList[i]["BoxCollider"];

			boxCollider->isTrigger = BoxCollider["m_IsTrigger"].as<bool>();

			YAML::Node m_Size = BoxCollider["m_Size"];

			boxCollider->size.x = m_Size["x"].as<float>();
			boxCollider->size.y = m_Size["y"].as<float>();
			boxCollider->size.z = m_Size["z"].as<float>();

			YAML::Node m_Center = BoxCollider["m_Center"];

			boxCollider->center.x = m_Center["x"].as<float>();
			boxCollider->center.y = m_Center["y"].as<float>();
			boxCollider->center.z = m_Center["z"].as<float>();

			scene.gameObjects.back().boxCollider = boxCollider;
		}
		else if (yaml->yamlNodeList[i]["SphereCollider"])
		{
			shared_ptr<YAMLBinaryData::SphereCollider> sphereCollider = std::make_shared<YAMLBinaryData::SphereCollider>();

			YAML::Node SphereCollider = yaml->yamlNodeList[i]["SphereCollider"];

			sphereCollider->isTrigger = SphereCollider["m_IsTrigger"].as<bool>();

			sphereCollider->radius = SphereCollider["m_Radius"].as<float>();

			YAML::Node m_Center = SphereCollider["m_Center"];

			sphereCollider->center.x = m_Center["x"].as<float>();
			sphereCollider->center.y = m_Center["y"].as<float>();
			sphereCollider->center.z = m_Center["z"].as<float>();

			scene.gameObjects.back().sphereCollider = sphereCollider;
		}
		else if (yaml->yamlNodeList[i]["PrefabInstance"])
		{
			YAMLBinaryData::Prefab prefabInstance;

			YAMLBinaryData::Transform transform;

			YAML::Node PrefabInstance = yaml->yamlNodeList[i]["PrefabInstance"];

			YAML::Node m_Modification = PrefabInstance["m_Modification"];

			YAML::Node m_Modifications = m_Modification["m_Modifications"];

			for (YAML::const_iterator it = m_Modifications.begin(); it != m_Modifications.end(); ++it)
			{
				if (it->operator[]("propertyPath").as<std::string>().compare("m_Name") == 0)
				{
					prefabInstance.name = it->operator[]("value").as<std::string>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalPosition.x") == 0)
				{
					transform.localPosition.x = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalPosition.y") == 0)
				{
					transform.localPosition.y = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalPosition.z") == 0)
				{
					transform.localPosition.z = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalEulerAnglesHint.x") == 0)
				{
					transform.localRotation.x = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalEulerAnglesHint.y") == 0)
				{
					transform.localRotation.y = it->operator[]("value").as<float>();
				}
				else if (it->operator[]("propertyPath").as<std::string>().compare("m_LocalEulerAnglesHint.z") == 0)
				{
					transform.localRotation.z = it->operator[]("value").as<float>();
				}
			}

			prefabInstance.transform = transform;

			scene.prefabs.emplace_back(prefabInstance);
		}
	}

	std::string sceneName = "SampleScene";

	std::ofstream ofs(sceneName + ".noob", std::ios_base::binary);
	boost::iostreams::filtering_stream<boost::iostreams::output> buffer;
	buffer.push(boost::iostreams::zlib_compressor());
	buffer.push(ofs);
	boost::archive::binary_oarchive oa(buffer);

	oa << scene;

	yaml.reset();
}
