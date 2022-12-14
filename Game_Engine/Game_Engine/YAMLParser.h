#pragma once
#include "YAMLBinaryLayout.h"

struct YAMLNode
{
	std::string keyName = "none";

	std::string value = "none";

	std::vector<std::shared_ptr<YAMLNode>> childNodeList;
};

class YAMLParser
{
private:
	bool isCheck = false;

public:
	// TopNode �������� LoadAll �Ѵ�.
	std::vector<YAML::Node> yamlNodeList;

	// OpenFile �ϸ� ���⿡ ���� ��� ������ ����.
	std::vector<std::shared_ptr<YAMLNode>> nodeList;	

public:
	YAMLParser() {};
	~YAMLParser() {};

public:
	void OpenFile(const std::string& path);

	void ReadNode(YAML::Node yamlNode, std::shared_ptr<YAMLNode> nodeData);

	std::string ScalarNode(YAML::const_iterator node_it);

	void SequenceNode(YAML::const_iterator node_it, std::shared_ptr<YAMLNode> nodeData);
};
