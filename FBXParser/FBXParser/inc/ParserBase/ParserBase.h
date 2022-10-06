#pragma once
#include "FBXParser/FBXParserDLL.h"
#include "ParserData/ParserData.h"

class ParserBase
{
public:
	ParserBase();
	virtual ~ParserBase();

public:
	static FBXParser_DLL std::shared_ptr<ParserBase> Create();

	FBXParser_DLL virtual std::shared_ptr<FBXModel> LoadFbx(const std::string& path) abstract;

	FBXParser_DLL virtual void Release() abstract;
};

