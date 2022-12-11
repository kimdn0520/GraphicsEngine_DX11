#pragma once
#include "YAMLBinaryLayout.h"

/// <summary>
/// nodeList에 node들을 담아준다 (앞에 공백이 없던 최상위 노드들이 담긴다)
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
