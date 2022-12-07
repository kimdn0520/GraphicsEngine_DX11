#pragma once

class YAMLParser
{
private:
	YAML::Emitter emitter;				// 출력스트림 역할

	std::vector<YAML::Node> nodeList;

public:
	YAMLParser() {};
	~YAMLParser() {};

public:
	

};

