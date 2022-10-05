#include "pch.h"
#include "FBXParser/FBXParser.h"

FBXParser::FBXParser()
{
}

FBXParser::~FBXParser()
{
}

std::shared_ptr<FbxModel> FBXParser::LoadFbx(const std::wstring& path)
{
	return std::shared_ptr<FbxModel>();
}

void FBXParser::Release()
{
}
