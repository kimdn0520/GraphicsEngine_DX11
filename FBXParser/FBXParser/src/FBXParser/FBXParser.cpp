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

    fbxModel->fbxSkeletonInfo = std::make_shared<FBXSkeletonInfo>();

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

    ProcessNode(scene->mRootNode, scene);

    LoadMaterial(scene);

	return fbxModel;
}

void FBXParser::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Assimp의 각 노드는 mesh index들의 모음을 가지고 있다.
    // 각 index는 scene 객체 내부의 특정한 mesh를 가리킨다.
    for (size_t nodeMeshCnt = 0; nodeMeshCnt < node->mNumMeshes; nodeMeshCnt++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[nodeMeshCnt]];

        fbxModel->fbxMeshInfoList.emplace_back(LoadMeshInfo(node, mesh, scene));
    }

    // child Node가 있다면 재귀로 들어가준다.
    for (size_t nodeChildMeshCnt = 0; nodeChildMeshCnt < node->mNumChildren; nodeChildMeshCnt++)
    {
        ProcessNode(node->mChildren[nodeChildMeshCnt], scene);
    }
}

/// <summary>
/// mesh는 vertex배열과 bone배열을 가지고 있다.
/// 그렇다면.. 이 mesh의 boen들은.. 이 mesh를 skin으로 삼는것인가?
/// </summary>
std::shared_ptr<FBXMeshInfo> FBXParser::LoadMeshInfo(aiNode* node, aiMesh* mesh, const aiScene* scene)
{
    std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

    size_t vertexNum = mesh->mNumVertices;           // 현재 메시의 버텍스 총 갯수
        
    fbxMeshInfo->meshName = mesh->mName.C_Str();     // 현재 메시에 이름을 넣는다.

    fbxMeshInfo->nodeTM = ConvertMatrix(node->mTransformation);
    
    // 버텍스를 넣는다.
    for (int vertexCnt = 0; vertexCnt < vertexNum; vertexCnt++)
    {
		Vertex vertex;

		vertex.position = DirectX::SimpleMath::Vector3(mesh->mVertices[vertexCnt].x, mesh->mVertices[vertexCnt].y, mesh->mVertices[vertexCnt].z);
		
        if (mesh->HasVertexColors(0))
            vertex.color = DirectX::SimpleMath::Vector4(mesh->mColors[vertexCnt]->r, mesh->mColors[vertexCnt]->g, mesh->mColors[vertexCnt]->b, mesh->mColors[vertexCnt]->a);

        if(mesh->HasTextureCoords(0))
            vertex.uv = DirectX::SimpleMath::Vector2(mesh->mTextureCoords[0][vertexCnt].x, mesh->mTextureCoords[0][vertexCnt].y);
        else
			vertex.uv = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

        if(mesh->HasNormals())
		    vertex.normal = DirectX::SimpleMath::Vector3(mesh->mNormals[vertexCnt].x, mesh->mNormals[vertexCnt].y, mesh->mNormals[vertexCnt].z);
			
        if(mesh->HasTangentsAndBitangents())
            vertex.tangent = DirectX::SimpleMath::Vector3(mesh->mTangents[vertexCnt].x, mesh->mTangents[vertexCnt].y, mesh->mTangents[vertexCnt].z);
		
		/*DirectX::XMMATRIX worldTMInverse = DirectX::XMMatrixInverse(nullptr, fbxMeshInfo->nodeTM);
		DirectX::XMVECTOR vertexVec = { vertex.position.x, vertex.position.y, vertex.position.z };
		DirectX::XMVECTOR vertexXworldTM = XMVector3Transform(vertexVec, worldTMInverse);
		DirectX::XMFLOAT3 temp;
		XMStoreFloat3(&temp, vertexXworldTM);

		vertex.position.x = temp.x;
		vertex.position.y = temp.y;
		vertex.position.z = temp.z;*/
        
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

	/*if(mesh->HasBones())
		ExtractBoneWeight(mesh, scene, fbxMeshInfo);*/

    //LoadAnimation(scene);

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

        // Baser Color 텍스쳐라고 이름적혀있는 것들이 왜 이놈으로 불러와지는거임?
        // 근데 뭐 둘다 써놓으면 되서 상관없긴함
        // Diffuse Texture
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString texturePath;

			// 텍스쳐 파일의 위치(이름 포함)를 얻을 수 있다. 
			material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

			// 근데 나는 텍스쳐 위치 까지는 필요없기에.. 이름만 가져올거야
			std::string temp = texturePath.C_Str();

			//if (temp.find("REF") == std::string::npos)
			
			std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

            fbxMaterialInfo->isAlbedo = true;

            std::wstring wstr = L"";

            wstr.assign(textureName.begin(), textureName.end());

            fbxMaterialInfo->albedoMap = wstr;
		}

        /////////////////////////////////////////////////////////////////////
        //                         PBR Texture
        /////////////////////////////////////////////////////////////////////
        
        // Albedo Map
        if (material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
        {
			aiString texturePath;

			material->GetTexture(aiTextureType_NORMALS, 0, &texturePath);

			std::string temp = texturePath.C_Str();

			std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

			fbxMaterialInfo->isAlbedo = true;

			std::wstring wstr = L"";

			wstr.assign(textureName.begin(), textureName.end());

			fbxMaterialInfo->albedoMap = wstr;
        } 

        // Normal Map
        if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
			aiString texturePath;

			material->GetTexture(aiTextureType_NORMALS, 0, &texturePath);

			std::string temp = texturePath.C_Str();

			std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

			fbxMaterialInfo->isNormal = true;

			std::wstring wstr = L"";

			wstr.assign(textureName.begin(), textureName.end());

			fbxMaterialInfo->normalMap = wstr;
        }

        // Metallic Map
        if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
        {
            aiString texturePath;

            material->GetTexture(aiTextureType_METALNESS, 0, &texturePath);

            std::string temp = texturePath.C_Str();

            std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

            fbxMaterialInfo->isMetallic = true;

            std::wstring wstr = L"";

            wstr.assign(textureName.begin(), textureName.end());

            fbxMaterialInfo->metallicMap = wstr;
        }

        // Roughness Map
        if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
        {
            aiString texturePath;

            material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &texturePath);

            std::string temp = texturePath.C_Str();

            std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

            fbxMaterialInfo->isRoughness = true;

            std::wstring wstr = L"";

            wstr.assign(textureName.begin(), textureName.end());

            fbxMaterialInfo->roughnessMap = wstr;
        }

        // Ambient Occlusion Map
        if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
        {
            aiString texturePath;

            material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texturePath);

            std::string temp = texturePath.C_Str();

            std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

            fbxMaterialInfo->isAO = true;

            std::wstring wstr = L"";

            wstr.assign(textureName.begin(), textureName.end());

            fbxMaterialInfo->AOMap = wstr;
        }

        // Emissive Map
        if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
        {
            aiString texturePath;

            material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath);

            std::string temp = texturePath.C_Str();

            std::string textureName = temp.substr(temp.find_last_of("\\") + 1, temp.length() - temp.find_last_of("\\"));

            fbxMaterialInfo->isEmissive = true;

            std::wstring wstr = L"";

            wstr.assign(textureName.begin(), textureName.end());

            fbxMaterialInfo->emissiveMap = wstr;
        }

        // 모델의 머터리얼 리스트에 푸쉬
        fbxModel->materialList.emplace_back(fbxMaterialInfo);
    }
}

/// <summary>
/// 해당 Mesh의 Bone들과 Vertex를 연결해야한다.
/// 그러면서 vertex에 해당하는 가중치를 넣어준다.
/// aiMesh는 bone배열과 vertex배열을 가지고 있다.
/// [주의!] 
/// Assimp를 써서 로드하면 Bone은 Mesh 취급 안한다 (bone 도 GeomObject 취급하던 ASE랑 다르다)
/// </summary>
void FBXParser::ExtractBoneWeight(aiMesh* mesh, const aiScene* scene, std::shared_ptr<FBXMeshInfo>& fbxMeshInfo)
{
    int boneNum = mesh->mNumBones;                           // 이 메시에 포함된 bone의 수
    
    for (int boneCnt = 0; boneCnt < boneNum; boneCnt++)
    {
        int boneID = -1;

        auto vertex = mesh->mBones[boneCnt]->mWeights;       // 이 bone에 의해 영향을 받는 vertex들

        int vertexNum = mesh->mBones[boneCnt]->mNumWeights;  // 이 bone의 영향을 받는 vertex 수

        // BoneMap에 Bone 정보가 추가되지 않았다면
        if (boneMap.find(mesh->mBones[boneCnt]->mName.C_Str()) == boneMap.end())
        {
            boneID = boneCounter;

		    std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

		    fbxBoneInfo->boneID = boneCounter;                              // 본 id

		    fbxBoneInfo->boneName = mesh->mBones[boneCnt]->mName.C_Str();   // 본의 이름

		    fbxBoneInfo->offsetMatrix = ConvertMatrix(mesh->mBones[boneCnt]->mOffsetMatrix);

            // BoneMap에 Bone 정보 추가
            boneMap.insert(std::make_pair(fbxBoneInfo->boneName, fbxBoneInfo));
            
            // FBXModel Skeleton에 Bone 정보  추가
            fbxModel->fbxSkeletonInfo->fbxBoneInfoList.emplace_back(fbxBoneInfo);

            boneCounter++;
        }
        // Bone 정보가 이미 있다면
        else
        {
            boneID = boneMap[mesh->mBones[boneCnt]->mName.C_Str()]->boneID;
        }

        // 해당 bone에 영향을 받는 vertex들을 돌거야
        // 해당 메시가 bone에 영향을 받는다면 for문을 돌고 아니면 안돈다.
        // 돈다면 스키닝 메시이다.
        for (int vertexCnt = 0; vertexCnt < vertexNum; vertexCnt++)
        {
			fbxModel->isSkinnedAnimation = true;            // 어쨌든 스키닝 애니메이션이 있다고하자

			fbxMeshInfo->isSkinned = true;                  // 이 메시는 스킨드 메시이다.

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
    }
}

/// <summary>
/// Rotation의 쿼터니언 값은 이전 프레임의 누적된 값을 제공하므로 그대로 사용하면 된다.
/// 쿼터니언 값의 순서는 그래픽스 개념에 맞춰 wxyz순이므로 DX를 사용할 시 유의
/// 애니메이션 값은 로컬 행렬이 연산된 값이므로 최종 행렬을 만들때 로컬행렬을 고려할 필요 없다.
/// 디버그 모드에서는 키값이 없어도 정보를 채워 pos, scale, rot 모든 키 값이 프레임에 맞춰 1:1 매칭되어 나오지만
/// 릴리즈 모드에서는 키 값이 없는 경우는 추가하지 않아 각각 다르게 나올 수 있다.
/// </summary>
void FBXParser::LoadAnimation(const aiScene* scene)
{
    // 애니메이션의 갯수 만큼
    for (int i = 0; i < scene->mNumAnimations; i++) 
    {
        auto aiAni = scene->mAnimations[i];

        float lastTime = 0.f;
        
        std::shared_ptr<FBXAnimClipInfo> aniInfo = std::make_shared<FBXAnimClipInfo>();
        aniInfo->duration = ((float)aiAni->mDuration);
        aniInfo->ticksPerFrame = ((float)aiAni->mTicksPerSecond);

        std::string aniName = aiAni->mName.C_Str();

        // 연결된 노드 만큼...
        for (UINT j = 0; j < aiAni->mNumChannels; j++) 
        {
            auto aiAniNode = aiAni->mChannels[j];

            std::shared_ptr<FBXKeyFrameInfo> aniKeyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

            // pos, rot, scale key중에 가장 큰걸루다가 하자
            UINT keyCnt = std::max(aiAniNode->mNumPositionKeys, aiAniNode->mNumRotationKeys);
            keyCnt = std::max(keyCnt, aiAniNode->mNumScalingKeys);

            // 키 프레임 정보를 구성한다.
            DirectX::SimpleMath::Vector3 translation = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
            DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
            DirectX::SimpleMath::Vector4 rotation = DirectX::SimpleMath::Vector4(0.f, 0.f, 0.f, 0.f);
            float	 time = 0.f;

            // 키 값은 있을 수도 있으며, 없을수도 있다.
            // 없는 경우 이전 값으로 채운다.
            for (UINT k = 0; k < keyCnt; k++)
            {
                if (aiAniNode->mNumPositionKeys > k) 
                {
                    auto posKey = aiAniNode->mPositionKeys[k];
                    memcpy_s(&translation, sizeof(translation), &posKey.mValue, sizeof(posKey.mValue));
                    time = (float)aiAniNode->mPositionKeys[k].mTime;
                }

                if (aiAniNode->mNumRotationKeys > k)
                {
                    auto rotKey = aiAniNode->mRotationKeys[k];
                    rotation = DirectX::SimpleMath::Vector4(rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w);
                    time = (float)aiAniNode->mRotationKeys[k].mTime;
                }

                if (aiAniNode->mNumScalingKeys > k)
                {
                    auto scaleKey = aiAniNode->mScalingKeys[k];
                    memcpy_s(&scale, sizeof(scale), &scaleKey.mValue, sizeof(scaleKey.mValue));
                    time = (float)aiAniNode->mScalingKeys[k].mTime;
                }

                aniKeyFrameInfo->time = time;
                aniKeyFrameInfo->transform = translation;
                aniKeyFrameInfo->rotation = rotation;
                aniKeyFrameInfo->scale = scale;
            }

            aniInfo->keyFrameList.emplace_back(aniKeyFrameInfo);
            
            lastTime = std::max(aniInfo->keyFrameList.back()->time, lastTime);
        }

        aniInfo->endKeyFrame = lastTime;

        // 모델의 애니메이션 리스트에 추가
        fbxModel->animationClipList.emplace_back(aniInfo);
    }
}

void FBXParser::Release()
{
    
}
