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
	char line[256] = { 0 };	// 한 줄

public:
	// TopNode 정보들을 LoadAll 한다.
	std::vector<YAML::Node> yamlNodeList;
	
	// UnityScene 전용 ID 담기
	std::vector<std::string> fildIDList;

public:
	YAMLParser() {};
	~YAMLParser() {};

public:
	void OpenFile(const std::string& path);

	//void ReadNode(YAML::Node yamlNode, std::shared_ptr<YAMLNode> nodeData);

	//std::string ScalarNode(YAML::const_iterator node_it);

	//void SequenceNode(YAML::const_iterator node_it, std::shared_ptr<YAMLNode> nodeData);

	//void Release(std::vector<std::shared_ptr<YAMLNode>> list);
};
