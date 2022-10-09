#include "pch.h"
#include "FBXParser/FBXParser.h"
#include "ParserData/ParserData.h"

FBXParser::FBXParser()
{}

FBXParser::~FBXParser()
{}

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
        
        fbxModel->fbxMeshInfoList.emplace_back(LoadMeshInfo(mesh, scene));
    }

    // child Node�� �ִٸ� ��ͷ� ���ش�.
    for (size_t nodeChildMeshCnt = 0; nodeChildMeshCnt < node->mNumChildren; nodeChildMeshCnt++)
    {
        ParseNode(node->mChildren[nodeChildMeshCnt], scene);
    }
}

/// <summary>
/// mesh�� vertex�迭�� bone�迭�� ������ �ִ�.
/// �׷��ٸ�.. �� mesh�� boen����.. �� mesh�� skin���� ��°��ΰ�?
/// </summary>
std::shared_ptr<FBXMeshInfo> FBXParser::LoadMeshInfo(aiMesh* mesh, const aiScene* scene)
{
    std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

    size_t vertexNum = mesh->mNumVertices;           // ���� �޽��� ���ؽ� �� ����
        
    fbxMeshInfo->meshName = mesh->mName.C_Str();     // ���� �޽ÿ� �̸��� �ִ´�.
       
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
       fbxMeshInfo->meshVertexList.emplace_back(vertex);
    }

    size_t faceNum = mesh->mNumFaces;                // ���� �޽��� face ����
        
    // �ε����� �ִ´�.
    for (int faceCnt = 0; faceCnt < faceNum; faceCnt++)
    {
        const aiFace face = mesh->mFaces[faceCnt];

		fbxMeshInfo->indices.emplace_back(face.mIndices[0]);
		fbxMeshInfo->indices.emplace_back(face.mIndices[1]);
		fbxMeshInfo->indices.emplace_back(face.mIndices[2]);
    }

    // ���͸��� �̸��� �ִ´�.
    // assimp������ mesh �Ѱ��� material �Ѱ� �̴�.
    // fbx�� mesh�ϳ��� material ������ ��������� mesh�� �ɰ������ٰ���.
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        fbxMeshInfo->materialName = material->GetName().C_Str();
    }

    ExtractBoneWeight(mesh, scene, fbxMeshInfo);

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

        // Albedo Map
        if (material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
        {
            aiString texturePath;

            // �ؽ��� ������ ��ġ(�̸� ����)�� ���� �� �ִ�. 
            material->GetTexture(aiTextureType_BASE_COLOR, 0, &texturePath);        
        
            // �ٵ� ���� �ؽ��� ��ġ ������ �ʿ���⿡.. �̸��� �����ðž�
            std::string temp = texturePath.C_Str();

            // �ϴ� �Ľ��� �غ��鼭 Ȯ������ ��ġ�� ������ �ؼ� ����������..?
            //std::string textureName = temp.substr()  find_last_of ...
        }

        // Normal Map
        if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
        {

        }

        // Metallic Map
        if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
        {

        }

        // Roughness Map
        if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
        {

        }

        // Ambient Occlusion Map
        if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
        {

        }

        // Emissive Map
        if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
        {
            
        }

        // ���� ���͸��� ����Ʈ�� Ǫ��
        fbxModel->materialList.emplace_back(fbxMaterialInfo);
    }
}

/// <summary>
/// �ش� Mesh�� Bone��� Vertex�� �����ؾ��Ѵ�.
/// �׷��鼭 vertex�� �ش��ϴ� ����ġ�� �־��ش�.
/// </summary>
void FBXParser::ExtractBoneWeight(aiMesh* mesh, const aiScene* scene, std::shared_ptr<FBXMeshInfo>& fbxMeshInfo)
{
    size_t boneNum = mesh->mNumBones;                        // �� �޽ÿ� ���Ե� bone�� ��

    for (int boneCnt = 0; boneCnt < boneNum; boneCnt++)
    {
        std::string boneName = mesh->mBones[boneCnt]->mName.C_Str();    // �ش� ���� �̸�

        auto vertex = mesh->mBones[boneCnt]->mWeights;       // �� bone�� ���� ������ �޴� vertex��?

        int vertexNum = mesh->mBones[boneCnt]->mNumWeights;  // �� bone�� ������ �޴� vertex ��

        std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

        // �ش� bone�� ������ �޴� vertex���� ���ž�
        for (int vertexCnt = 0; vertexCnt < vertexNum; vertexCnt++)
        {
            int vertexID = vertex[vertexCnt].mVertexId;     // �ش� ���ؽ��� id

            float weight = vertex[vertexCnt].mWeight;       // �ش� ���ؽ��� ����ġ

            // �ִ� 8���� �Ұž�
            // ���鼭 ����� �ְ� break ��
            for (int i = 0; i < 8; i++)
            {
                if (fbxMeshInfo->meshVertexList[vertexID].boneIndices[i] == -1)
                {
                    fbxMeshInfo->meshVertexList[vertexID].weights[i] = weight;

                    fbxMeshInfo->meshVertexList[vertexID].boneIndices[i] = boneID;

                    break;
                }
            }
        }

        boneID++;
    }
}

void FBXParser::LoadAnimation(const aiScene* scene)
{

}

void FBXParser::Release()
{
    
}
