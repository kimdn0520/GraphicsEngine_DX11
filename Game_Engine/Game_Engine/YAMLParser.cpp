#include "pch.h"
#include "YAMLParser.h"

void YAMLParser::OpenFile(const std::string& path)
{
	// 모든 TopNode를 담는다.
	nodeList = YAML::LoadAllFromFile(path);

	// TopNode를 읽으면서 정보를 담자
	for (int i = 0; i < nodeList.size(); i++)
	{
		YAML::Node node = nodeList[i];

		ReadNode(node);
	}
}

void YAMLParser::ReadNode(YAML::Node node)
{
	YAML::const_iterator node_it = node.begin();

	// 노드를 순회한다.
	for (node_it; node_it != node.end(); node_it++)
	{
		YAML::Node key = node_it->first;

		YAML::Node value = node_it->second;

		string keyName = key.as<std::string>();

		// value 값에 따라 switch!
		switch (value.Type())
		{
		case YAML::NodeType::Undefined:
			break;
		case YAML::NodeType::Null:
			break;
		case YAML::NodeType::Map:			// key, value값
		{
			MapNode(node_it);
		}
		break;
		case YAML::NodeType::Scalar:		// 숫자, 문자
		{
			ScalarNode(node_it);
		}
		break;
		case YAML::NodeType::Sequence:		// 배열, 리스트
		{
			SequenceNode(node_it);
		}
		break;
		default:
			break;
		}
	}
}

void YAMLParser::MapNode(YAML::const_iterator node_it)
{
	YAML::Node value = node_it->second;

	ReadNode(value);
}

void YAMLParser::ScalarNode(YAML::const_iterator node_it)
{
	string stringData = node_it->second.as<std::string>();
}

void YAMLParser::SequenceNode(YAML::const_iterator node_it)
{
	YAML::Node value = node_it->second;

	int size = (int)value.size();
	
	for (int i = 0; i < size; i++)
	{
		YAML::Node temp = value[i];
		
		YAML::const_iterator Nowit = temp.begin();
		
		for (Nowit; Nowit != temp.end(); ++Nowit)
		{
			if (temp.IsScalar() == true) 
			{
				ScalarNode(Nowit);
			}
			else if (temp.IsMap() == true)
			{
				MapNode(Nowit);
			}
			else if (temp.IsSequence() == true)
			{
				SequenceNode(Nowit);
			}
		}
	}
}
