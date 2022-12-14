#include "pch.h"
#include "YAMLParser.h"

void YAMLParser::OpenFile(const std::string& path)
{
	// ��� TopNode�� ��´�.
	yamlNodeList = YAML::LoadAllFromFile(path);

	for (int i = 0; i < yamlNodeList.size(); i++)
	{	
		ReadNode(yamlNodeList[i], nullptr);
	}
}

void YAMLParser::ReadNode(YAML::Node yamlNode, std::shared_ptr<YAMLNode> nodeData)
{
	YAML::const_iterator node_it = yamlNode.begin();

	// ��带 ��ȸ�Ѵ�.
	for (node_it; node_it != yamlNode.end(); node_it++)
	{
		// ���ο� ��� ����
		std::shared_ptr<YAMLNode> node = std::make_shared<YAMLNode>();

		YAML::Node key = node_it->first;

		YAML::Node value = node_it->second;
		
		node->keyName = key.as<std::string>();

		// value ���� ���� switch!
		switch (value.Type())
		{
		case YAML::NodeType::Undefined:
			break;
		case YAML::NodeType::Null:
			break;
		case YAML::NodeType::Map:			// key, value��
		{
			// childNode�� �־��ش�
			if(nodeData != nullptr)
				nodeData->childNodeList.push_back(node);
			else
				nodeList.push_back(node);					// Top Node

			ReadNode(value, node);
		}
		break;
		case YAML::NodeType::Scalar:		// ����, ����
		{
			node->value = ScalarNode(node_it);

			// childNode�� �־��ش�
			if (nodeData != nullptr)
				nodeData->childNodeList.push_back(node);
		}
		break;
		case YAML::NodeType::Sequence:		// �迭, ����Ʈ
		{
			// childNode�� �־��ش�
			if (nodeData != nullptr)
				nodeData->childNodeList.push_back(node);

			SequenceNode(node_it, node);
		}
		break;
		default:
			break;
		}
	}
}

std::string YAMLParser::ScalarNode(YAML::const_iterator node_it)
{
	return node_it->second.as<std::string>();
}

void YAMLParser::SequenceNode(YAML::const_iterator node_it, std::shared_ptr<YAMLNode> nodeData)
{
	// �迭
	YAML::Node arr = node_it->second;

	for (int i = 0; i < (int)arr.size(); i++)
	{
		YAML::Node tmpNode = arr[i];
		
		// ��ü �迭
		// �迭�� �Ѱ��ٶ� tmp_it �� �Ѱ��ָ� ������ ����..
		// node ��ü�� �Ѱ��ִ� ������ �ߴ�.
		ReadNode(tmpNode, nodeData);
	}
}
