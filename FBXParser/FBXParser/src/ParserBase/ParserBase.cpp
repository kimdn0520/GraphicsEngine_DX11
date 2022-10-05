#include "pch.h"
#include "ParserBase/ParserBase.h"
#include "FBXParser/FBXParser.h"

ParserBase::ParserBase()
{
}

ParserBase::~ParserBase()
{
}

FBXParser_DLL std::shared_ptr<ParserBase> ParserBase::Create()
{
    std::shared_ptr<ParserBase> newFbxLoader;

    newFbxLoader = std::make_shared<FBXParser>();

    return newFbxLoader;
}