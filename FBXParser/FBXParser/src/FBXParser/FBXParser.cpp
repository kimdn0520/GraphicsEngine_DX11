#include "pch.h"
#include "FBXParser/FBXParser.h"

FBXParser::FBXParser()
{
}

FBXParser::~FBXParser()
{
	
}

std::shared_ptr<FBXModel> FBXParser::LoadFbx(const std::string& filePath)
{
    fbxModel = std::make_shared<FBXModel>();

    scene = importer.ReadFile(filePath,
        aiProcess_CalcTangentSpace |                // 가져온 메쉬의 tangent 및 bitangent를 계산한다. 메쉬에 법선이 없으면 작업을 수행하지 않는다.
        aiProcess_Triangulate |                     // 3개 이상의 모서리를 가진 다각형 면을 삼각형으로 만든다.
        aiProcess_JoinIdenticalVertices |           // 동일한 꼭지점 결합, 인덱싱 최적화
        aiProcess_SortByPType);                     // 단일타입의 프리미티브로 구성된 '깨끗한' 매쉬를 만듬

    if (scene != nullptr)
    {
        
    }

	return fbxModel;
}

void FBXParser::LoadMesh()
{
    fbxModel->fbxMeshInfoList.resize(scene->mNumMeshes);    // 메시 크기만큼 resize

    size_t meshNum = scene->mNumMeshes;

    for (int i = 0; i < meshNum; i++)
    {
        const aiMesh* mesh = scene->mMeshes[i];

        // TODO : ..
    }
}

void FBXParser::Release()
{
}
