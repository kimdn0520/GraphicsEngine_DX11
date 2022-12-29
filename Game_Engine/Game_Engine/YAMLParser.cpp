#include "pch.h"
#include "YAMLParser.h"

void YAMLParser::OpenFile(const std::string& path)
{
	try
	{
		yamlNodeList = YAML::LoadAllFromFile(path);
	}
	catch (YAML::Exception& e)
	{
		std::cerr << "Error parsing YAML document : " << e.what() << std::endl;
	}
	
	/// <summary>
	/// yaml-cpp �δ� ����Ƽ�� --- !u!29 &1 �̰Ÿ� �Ľ��Ҽ��� ����.
	/// �׷��� ���� '---' �ؽ�Ʈ�� �о ID���� �־��ֵ��� ����
	/// </summary>
	FILE* file;
	fopen_s(&file, path.c_str(), "r");

	// feof(FILE* stream) - ������ ���� �����ϰԵǸ� 0�� �ƴ� ���� ��ȯ
	while (!feof(file))
	{
		fgets(line, 256, file);

		int ch_Index = 0;

		// --- �� ��츸 ����.
		if (line[ch_Index] == '-')
		{	
			// &�� ���ö����� ���� ���ڷ� ����.
			while (line[ch_Index] != '&')
			{
				ch_Index++;
			}

			ch_Index++;

			std::string fildID = "";
			int index = 0;

			while (line[ch_Index] >= '0' && line[ch_Index] <= '9')
			{
				fildID += line[ch_Index];
				ch_Index++;
			}

			fildIDList.emplace_back(fildID);
		}
	}
	
	// ���ʿ䰡..?
	/*for (int i = 0; i < yamlNodeList.size(); i++)
	{
		ReadNode(yamlNodeList[i], nullptr);
	}*/
}

//void YAMLParser::ReadNode(YAML::Node yamlNode, std::shared_ptr<YAMLNode> nodeData)
//{
//	YAML::const_iterator node_it = yamlNode.begin();
//
//	// ��带 ��ȸ�Ѵ�.
//	for (node_it; node_it != yamlNode.end(); node_it++)
//	{
//		// ���ο� ��� ����
//		std::shared_ptr<YAMLNode> node = std::make_shared<YAMLNode>();
//
//		YAML::Node key = node_it->first;
//
//		YAML::Node value = node_it->second;
//		
//		node->keyName = key.as<std::string>();
//
//		// value ���� ���� switch!
//		switch (value.Type())
//		{
//		case YAML::NodeType::Undefined:
//			break;
//		case YAML::NodeType::Null:
//			break;
//		case YAML::NodeType::Map:			// key, value��
//		{
//			// childNode�� �־��ش�
//			if(nodeData != nullptr)
//				nodeData->childNodeList.push_back(node);
//			else
//				nodeList.push_back(node);					// Top Node
//
//			ReadNode(value, node);
//		}
//		break;
//		case YAML::NodeType::Scalar:		// ����, ����
//		{
//			node->value = ScalarNode(node_it);
//
//			// childNode�� �־��ش�
//			if (nodeData != nullptr)
//				nodeData->childNodeList.push_back(node);
//		}
//		break;
//		case YAML::NodeType::Sequence:		// �迭, ����Ʈ
//		{
//			// childNode�� �־��ش�
//			if (nodeData != nullptr)
//				nodeData->childNodeList.push_back(node);
//
//			SequenceNode(node_it, node);
//		}
//		break;
//		default:
//			break;
//		}
//	}
//}
//
//std::string YAMLParser::ScalarNode(YAML::const_iterator node_it)
//{
//	return node_it->second.as<std::string>();
//}
//
//void YAMLParser::SequenceNode(YAML::const_iterator node_it, std::shared_ptr<YAMLNode> nodeData)
//{
//	// �迭
//	YAML::Node arr = node_it->second;
//
//	for (int i = 0; i < (int)arr.size(); i++)
//	{
//		YAML::Node tmpNode = arr[i];
//		
//		// ��ü �迭
//		// �迭�� �Ѱ��ٶ� tmp_it �� �Ѱ��ָ� ������ ����..
//		// node ��ü�� �Ѱ��ִ� ������ �ߴ�.
//		ReadNode(tmpNode, nodeData);
//	}
//}
//
//void YAMLParser::Release(std::vector<std::shared_ptr<YAMLNode>> list)
//{
//	for (auto& node : list)
//	{
//		if (!node->childNodeList.empty())
//		{
//			Release(node->childNodeList);
//		}
//
//		node.reset();
//	}
//}