#include "pch.h"
#include "YAMLParser.h"

void YAMLParser::OpenFile(const std::string& path)
{
	// 모든 TopNode를 담는다.
	yamlNodeList = YAML::LoadAllFromFile(path);

	for (int i = 0; i < yamlNodeList.size(); i++)
	{	
		ReadNode(yamlNodeList[i], nullptr);
	}
}

void YAMLParser::ReadNode(YAML::Node yamlNode, std::shared_ptr<YAMLNode> nodeData)
{
	YAML::const_iterator node_it = yamlNode.begin();

	// 노드를 순회한다.
	for (node_it; node_it != yamlNode.end(); node_it++)
	{
		// 새로운 노드 생성
		std::shared_ptr<YAMLNode> node = std::make_shared<YAMLNode>();

		YAML::Node key = node_it->first;

		YAML::Node value = node_it->second;
		
		node->keyName = key.as<std::string>();

		// value 값에 따라 switch!
		switch (value.Type())
		{
		case YAML::NodeType::Undefined:
			break;
		case YAML::NodeType::Null:
			break;
		case YAML::NodeType::Map:			// key, value값
		{
			// childNode로 넣어준다
			if(nodeData != nullptr)
				nodeData->childNodeList.push_back(node);
			else
				nodeList.push_back(node);					// Top Node

			ReadNode(value, node);
		}
		break;
		case YAML::NodeType::Scalar:		// 숫자, 문자
		{
			node->value = ScalarNode(node_it);

			// childNode로 넣어준다
			if (nodeData != nullptr)
				nodeData->childNodeList.push_back(node);
		}
		break;
		case YAML::NodeType::Sequence:		// 배열, 리스트
		{
			// childNode로 넣어준다
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
	// 배열
	YAML::Node arr = node_it->second;

	for (int i = 0; i < (int)arr.size(); i++)
	{
		YAML::Node tmpNode = arr[i];
		
		// 객체 배열
		// 배열을 넘겨줄때 tmp_it 을 넘겨주면 문제가 생김..
		// node 자체를 넘겨주는 것으로 했다.
		ReadNode(tmpNode, nodeData);
	}
}
