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
        aiProcess_SortByPType |                     // ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����
        aiProcess_ConvertToLeftHanded |             // D3D�� �޼���ǥ��� ��ȯ
        aiProcess_GenNormals |                      // �븻���� ���� 
        aiProcess_OptimizeMeshes                    // �޽ø� ����ȭ�Ͽ� �޽� ���� ���δ�.
        );

    // ���н�..
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        MessageBox(0, TEXT("LoadFbx Failed."), 0, 0);
    }

    fbxModel->fbxMeshInfoList.resize(scene->mNumMeshes);    // �޽� ũ�⸸ŭ resize

    ParseNode(scene->mRootNode, scene);

    LoadMaterial(scene);

	return fbxModel;
}

void FBXParser::ParseNode(aiNode* node, const aiScene* scene)
{
    // Assimp�� �� ���� mesh index���� ������ ������ �ִ�.
    // �� index�� scene ��ü ������ Ư���� mesh�� ����Ų��.
    for (size_t nodeMeshCnt = 0; nodeMeshCnt < node->mNumMeshes; nodeMeshCnt++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[nodeMeshCnt]];
        
        fbxModel->fbxMeshInfoList.push_back(LoadMeshInfo(mesh, scene));
    }

    // child Node�� �ִٸ� ��ͷ� ���ش�.
    for (size_t nodeChildMeshCnt = 0; nodeChildMeshCnt < node->mNumChildren; nodeChildMeshCnt++)
    {
        ParseNode(node->mChildren[nodeChildMeshCnt], scene);
    }
}

std::shared_ptr<FBXMeshInfo> FBXParser::LoadMeshInfo(aiMesh* mesh, const aiScene* scene)
{
    std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

    size_t meshNum = scene->mNumMeshes;

    for (int meshCnt = 0; meshCnt < meshNum; meshCnt++)
    {
        size_t vertexNum = scene->mMeshes[meshCnt]->mNumVertices;           // ���� �޽��� ���ؽ� �� ����
        
        fbxModel->fbxMeshInfoList[meshCnt]->meshName = mesh->mName.C_Str(); // ���� �޽ÿ� �̸��� �ִ´�.

        // ���ؽ��� �ִ´�.
        for (int vertexCnt = 0; vertexCnt < vertexNum; vertexCnt++)
        {
			Vertex vertex;

			vertex.position = DirectX::SimpleMath::Vector3(mesh->mVertices[vertexCnt].x, mesh->mVertices[vertexCnt].y, mesh->mVertices[vertexCnt].z);
			
            if(mesh->HasTextureCoords(0))
                vertex.uv = DirectX::SimpleMath::Vector2(mesh->mTextureCoords[0][vertexCnt].x, mesh->mTextureCoords[0][vertexCnt].y);
            else
				vertex.uv = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

			vertex.normal = DirectX::SimpleMath::Vector3(mesh->mNormals[vertexCnt].x, mesh->mNormals[vertexCnt].y, mesh->mNormals[vertexCnt].z);
			
            vertex.tangent = DirectX::SimpleMath::Vector3(mesh->mTangents[vertexCnt].x, mesh->mTangents[vertexCnt].y, mesh->mTangents[vertexCnt].z);
            
            // ���� �޽ÿ� �ش��ϴ� vertex �ϳ��� �ִ´�.
            fbxModel->fbxMeshInfoList[meshCnt]->meshVertexList.emplace_back(vertex);
        }

        size_t faceNum = scene->mMeshes[meshCnt]->mNumFaces;
        
        // �ε����� �ִ´�.
        for (int faceCnt = 0; faceCnt < faceNum; faceCnt++)
        {
            const aiFace face = mesh->mFaces[faceCnt];

			fbxModel->fbxMeshInfoList[meshCnt]->indices.emplace_back(face.mIndices[0]);
			fbxModel->fbxMeshInfoList[meshCnt]->indices.emplace_back(face.mIndices[1]);
			fbxModel->fbxMeshInfoList[meshCnt]->indices.emplace_back(face.mIndices[2]);
        }

        // ���͸��� �̸��� �ִ´�.
        // assimp������ mesh �Ѱ��� material �Ѱ� �̴�.
        // fbx�� mesh�ϳ��� material ������ ��������� mesh�� �ɰ������ٰ���.
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            fbxModel->fbxMeshInfoList[meshCnt]->materialName = material->GetName().C_Str();
        }
    }

    return fbxMeshInfo;
}

void FBXParser::LoadMaterial(const aiScene* scene)
{
    size_t materialNum = scene->mNumMaterials;           // ���͸����� �� ����

    for (int materialCnt = 0; materialCnt < materialNum; materialCnt++)
    {
        std::shared_ptr<FBXMaterialInfo> fbxMaterialInfo = std::make_shared<FBXMaterialInfo>();

        aiMaterial* material = scene->mMaterials[materialCnt];

        fbxMaterialInfo->materialName = material->GetName().C_Str();

        // PBR Texture
        if (material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
        {
            material->GetTexture(aiTextureType_BASE_COLOR, );
        }
    }
}

void FBXParser::Release()
{
}
