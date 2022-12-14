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
	// TopNode 정보들을 LoadAll 한다.
	std::vector<YAML::Node> yamlNodeList;

	// OpenFile 하면 여기에 이제 모든 정보가 담긴다.
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
