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
        
        fbxModel->fbxMeshInfoList.emplace_back(LoadMeshInfo(mesh, scene));
    }

    // child Node가 있다면 재귀로 들어가준다.
    for (size_t nodeChildMeshCnt = 0; nodeChildMeshCnt < node->mNumChildren; nodeChildMeshCnt++)
    {
        ParseNode(node->mChildren[nodeChildMeshCnt], scene);
    }
}

/// <summary>
/// mesh는 vertex배열과 bone배열을 가지고 있다.
/// 그렇다면.. 이 mesh의 boen들은.. 이 mesh를 skin으로 삼는것인가?
/// </summary>
std::shared_ptr<FBXMeshInfo> FBXParser::LoadMeshInfo(aiMesh* mesh, const aiScene* scene)
{
    std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

    size_t vertexNum = mesh->mNumVertices;           // 현재 메시의 버텍스 총 갯수
        
    fbxMeshInfo->meshName = mesh->mName.C_Str();     // 현재 메시에 이름을 넣는다.
       
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
       fbxMeshInfo->meshVertexList.emplace_back(vertex);
    }

    size_t faceNum = mesh->mNumFaces;                // 현재 메시의 face 정보
        
    // 인덱스를 넣는다.
    for (int faceCnt = 0; faceCnt < faceNum; faceCnt++)
    {
        const aiFace face = mesh->mFaces[faceCnt];

		fbxMeshInfo->indices.emplace_back(face.mIndices[0]);
		fbxMeshInfo->indices.emplace_back(face.mIndices[1]);
		fbxMeshInfo->indices.emplace_back(face.mIndices[2]);
    }

    // 머터리얼 이름을 넣는다.
    // assimp에서는 mesh 한개당 material 한개 이다.
    // fbx에 mesh하나에 material 여러개 였던경우라면 mesh를 쪼개버린다고함.
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
    size_t materialNum = scene->mNumMaterials;           // 메터리얼의 총 갯수

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

            // 텍스쳐 파일의 위치(이름 포함)를 얻을 수 있다. 
            material->GetTexture(aiTextureType_BASE_COLOR, 0, &texturePath);        
        
            // 근데 나는 텍스쳐 위치 까지는 필요없기에.. 이름만 가져올거야
            std::string temp = texturePath.C_Str();

            // 일단 파싱을 해보면서 확인하자 위치를 어디까지 해서 가져오는지..?
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

        // 모델의 머터리얼 리스트에 푸쉬
        fbxModel->materialList.emplace_back(fbxMaterialInfo);
    }
}

/// <summary>
/// 해당 Mesh의 Bone들과 Vertex를 연결해야한다.
/// 그러면서 vertex에 해당하는 가중치를 넣어준다.
/// </summary>
void FBXParser::ExtractBoneWeight(aiMesh* mesh, const aiScene* scene, std::shared_ptr<FBXMeshInfo>& fbxMeshInfo)
{
    size_t boneNum = mesh->mNumBones;                        // 이 메시에 포함된 bone의 수

    for (int boneCnt = 0; boneCnt < boneNum; boneCnt++)
    {
        std::string boneName = mesh->mBones[boneCnt]->mName.C_Str();    // 해당 본의 이름

        auto vertex = mesh->mBones[boneCnt]->mWeights;       // 이 bone에 의해 영향을 받는 vertex들?

        int vertexNum = mesh->mBones[boneCnt]->mNumWeights;  // 이 bone의 영향을 받는 vertex 수

        std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

        // 해당 bone에 영향을 받는 vertex들을 돌거야
        for (int vertexCnt = 0; vertexCnt < vertexNum; vertexCnt++)
        {
            int vertexID = vertex[vertexCnt].mVertexId;     // 해당 버텍스의 id

            float weight = vertex[vertexCnt].mWeight;       // 해당 버텍스의 가중치

            // 최대 8개로 할거양
            // 돌면서 빈곳에 넣고 break 함
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
