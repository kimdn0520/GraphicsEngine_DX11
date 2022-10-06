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
        aiProcess_CalcTangentSpace |                // ������ �޽��� tangent �� bitangent�� ����Ѵ�. �޽��� ������ ������ �۾��� �������� �ʴ´�.
        aiProcess_Triangulate |                     // 3�� �̻��� �𼭸��� ���� �ٰ��� ���� �ﰢ������ �����.
        aiProcess_JoinIdenticalVertices |           // ������ ������ ����, �ε��� ����ȭ
        aiProcess_SortByPType);                     // ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����

    if (scene != nullptr)
    {
        
    }

	return fbxModel;
}

void FBXParser::LoadMesh()
{
    fbxModel->fbxMeshInfoList.resize(scene->mNumMeshes);    // �޽� ũ�⸸ŭ resize

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
