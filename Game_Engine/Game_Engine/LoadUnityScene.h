#pragma once
#include "YAMLParser.h"

class LoadUnityScene
{
public:
	LoadUnityScene() {};
	~LoadUnityScene() {};

public:
	void LoadScene(const std::string& path);

	std::shared_ptr<YAMLNode> FindNode(std::shared_ptr<YAMLNode> findNode, std::string nodeName);
};

