#pragma once
#include "YAMLBinaryLayout.h"

/// <summary>
/// nodeList�� node���� ����ش� (�տ� ������ ���� �ֻ��� ������ ����)
/// 
/// </summary>
class YAMLParser
{
private:
	std::vector<YAML::Node> nodeList;

	

public:
	YAMLParser() {};
	~YAMLParser() {};

public:
	void OpenFile(const std::string& path);

	void ReadNode(YAML::Node node);

	void MapNode(YAML::const_iterator node_it);

	void ScalarNode(YAML::const_iterator node_it);

	void SequenceNode(YAML::const_iterator node_it);
};
