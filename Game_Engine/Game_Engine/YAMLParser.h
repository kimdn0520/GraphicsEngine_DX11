#pragma once

class YAMLParser
{
private:
	std::vector<YAML::Node> nodeList;

public:
	YAMLParser() {};
	~YAMLParser() {};

public:
	void OpenFile(const std::string& path);

};

