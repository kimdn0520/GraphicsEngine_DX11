#include "pch.h"
#include "YAMLParser.h"

void YAMLParser::OpenFile(const std::string& path)
{
	nodeList = YAML::LoadAllFromFile(path);
}
