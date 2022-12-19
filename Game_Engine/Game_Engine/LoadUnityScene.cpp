#include "pch.h"
#include "LoadUnityScene.h"
#include "YAMLParser.h"

void LoadUnityScene::LoadScene(const std::string& path)
{
	shared_ptr<YAMLParser> yaml = make_shared<YAMLParser>();

	yaml->OpenFile(path);

	YAMLBinaryData::Scene scene;

	for (int i = 0; i < yaml->nodeList.size(); i++)
	{
		if (yaml->nodeList[i]->keyName.compare("GameObject") == 0)
		{
			YAMLBinaryData::GameObject gameObject;

			std::shared_ptr<YAMLNode> m_Name;

			FindNode(yaml->nodeList[i], "m_Name", m_Name);

			gameObject.name = m_Name->value;

			scene.gameObjects.emplace_back(gameObject);
		}
		else if (yaml->nodeList[i]->keyName.compare("Transform") == 0)
		{
			YAMLBinaryData::Transform transform;

			std::shared_ptr<YAMLNode> m_LocalPosition;
			FindNode(yaml->nodeList[i], "m_LocalPosition", m_LocalPosition);
			float pos[3];

			for (int i = 0; i < m_LocalPosition->childNodeList.size(); i++)
			{
				pos[i] = stof(m_LocalPosition->childNodeList[i]->value);
			}

			transform.localPosition.x = pos[0];
			transform.localPosition.y = pos[1];
			transform.localPosition.z = pos[2];

			std::shared_ptr<YAMLNode> m_LocalRotation;
			FindNode(yaml->nodeList[i], "m_LocalRotation", m_LocalRotation);
			float rot[4];

			for (int i = 0; i < m_LocalRotation->childNodeList.size(); i++)
			{
				rot[i] = stof(m_LocalRotation->childNodeList[i]->value);
			}

			transform.localRotation.x = rot[0];
			transform.localRotation.y = rot[1];
			transform.localRotation.z = rot[2];

			std::shared_ptr<YAMLNode> m_LocalScale;
			FindNode(yaml->nodeList[i], "m_LocalScale", m_LocalScale);
			float scale[3];

			for (int i = 0; i < m_LocalScale->childNodeList.size(); i++)
			{
				scale[i] = stof(m_LocalScale->childNodeList[i]->value);
			}

			transform.localScale.x = scale[0];
			transform.localScale.y = scale[1];
			transform.localScale.z = scale[2];

			scene.gameObjects.back().transform = transform;
		}
		else if (yaml->nodeList[i]->keyName.compare("BoxCollider") == 0)
		{

		}
		else if (yaml->nodeList[i]->keyName.compare("CircleCollider") == 0)
		{

		}
		else if (yaml->nodeList[i]->keyName.compare("Camera") == 0)
		{

		}
		else if (yaml->nodeList[i]->keyName.compare("Light") == 0)
		{

		}
		else if (yaml->nodeList[i]->keyName.compare("Prefab") == 0)
		{

		}
	}

	yaml->Release();
}

void LoadUnityScene::FindNode(std::shared_ptr<YAMLNode> findNode, std::string nodeName, std::shared_ptr<YAMLNode>& node)
{
	for (auto& child : findNode->childNodeList)
	{
		if (child->keyName.compare(nodeName) == 0)
		{
			node = child;

			return;
		}

		FindNode(child, nodeName, node);
	}
}
