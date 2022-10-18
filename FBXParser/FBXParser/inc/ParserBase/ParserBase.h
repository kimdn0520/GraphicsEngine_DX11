#pragma once
#include "FBXParserDLL.h"

struct FBXModel;

class ParserBase
{
public:
	FBXParser_DLL ParserBase();
	FBXParser_DLL virtual ~ParserBase();

public:
	static FBXParser_DLL std::shared_ptr<ParserBase> Create();

	FBXParser_DLL virtual std::shared_ptr<FBXModel> LoadFbx(const std::string& path) abstract;
};

