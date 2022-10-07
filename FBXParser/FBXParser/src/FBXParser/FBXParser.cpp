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
        aiProcess_SortByPType |                     // 단일타입의 프리미티브로 구성된 '깨끗한' 매쉬를 만듬
        aiProcess_ConvertToLeftHanded |             // D3D의 왼손좌표계로 변환
        aiProcess_GenNormals |                      // 노말벡터 생성 
        aiProcess_OptimizeMeshes                    // 메시를 최적화하여 메쉬 수를 줄인다.
        );

    // 실패시..
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        MessageBox(0, TEXT("LoadFbx Failed."), 0, 0);
    }

    fbxModel->fbxMeshInfoList.resize(scene->mNumMeshes);    // 메시 크기만큼 resize

    ParseNode(scene->mRootNode, scene);

    LoadMaterial(scene);

	return fbxModel;
}

void FBXParser::ParseNode(aiNode* node, const aiScene* scene)
{
    // Assimp의 각 노드는 mesh index들의 모음을 가지고 있다.
    // 각 index는 scene 객체 내부의 특정한 mesh를 가리킨다.
    for (size_t nodeMeshCnt = 0; nodeMeshCnt < node->mNumMeshes; nodeMeshCnt++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[nodeMeshCnt]];
        
        fbxModel->fbxMeshInfoList.push_back(LoadMeshInfo(mesh, scene));
    }

    // child Node가 있다면 재귀로 들어가준다.
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
        size_t vertexNum = scene->mMeshes[meshCnt]->mNumVertices;           // 현재 메시의 버텍스 총 갯수
        
        fbxModel->fbxMeshInfoList[meshCnt]->meshName = mesh->mName.C_Str(); // 현재 메시에 이름을 넣는다.

        // 버텍스를 넣는다.
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
            
            // 현재 메시에 해당하는 vertex 하나를 넣는다.
            fbxModel->fbxMeshInfoList[meshCnt]->meshVertexList.emplace_back(vertex);
        }

        size_t faceNum = scene->mMeshes[meshCnt]->mNumFaces;
        
        // 인덱스를 넣는다.
        for (int faceCnt = 0; faceCnt < faceNum; faceCnt++)
        {
            const aiFace face = mesh->mFaces[faceCnt];

			fbxModel->fbxMeshInfoList[meshCnt]->indices.emplace_back(face.mIndices[0]);
			fbxModel->fbxMeshInfoList[meshCnt]->indices.emplace_back(face.mIndices[1]);
			fbxModel->fbxMeshInfoList[meshCnt]->indices.emplace_back(face.mIndices[2]);
        }

        // 머터리얼 이름을 넣는다.
        // assimp에서는 mesh 한개당 material 한개 이다.
        // fbx에 mesh하나에 material 여러개 였던경우라면 mesh를 쪼개버린다고함.
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
    size_t materialNum = scene->mNumMaterials;           // 메터리얼의 총 갯수

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
