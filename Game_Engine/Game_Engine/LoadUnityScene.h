#pragma once

class YAMLNode;

class LoadUnityScene
{

public:
	LoadUnityScene() {};
	~LoadUnityScene() {};

public:
	void LoadScene(const std::string& path);

	void FindNode(std::shared_ptr<YAMLNode> findNode, std::string nodeName, std::shared_ptr<YAMLNode>& node);
};

