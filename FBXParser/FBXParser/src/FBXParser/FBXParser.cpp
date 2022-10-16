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

    ProcessNode(scene->mRootNode, scene);

    LoadMaterial(scene);

	return fbxModel;
}

void FBXParser::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Assimp�� �� ���� mesh index���� ������ ������ �ִ�.
    // �� index�� scene ��ü ������ Ư���� mesh�� ����Ų��.
    for (size_t nodeMeshCnt = 0; nodeMeshCnt < node->mNumMeshes; nodeMeshCnt++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[nodeMeshCnt]];

        fbxModel->fbxMeshInfoList.emplace_back(LoadMeshInfo(node, mesh, scene));
    }

    // child Node�� �ִٸ� ��ͷ� ���ش�.
    for (size_t nodeChildMeshCnt = 0; nodeChildMeshCnt < node->mNumChildren; nodeChildMeshCnt++)
    {
        ProcessNode(node->mChildren[nodeChildMeshCnt], scene);
    }
}

/// <summary>
/// mesh�� vertex�迭�� bone�迭�� ������ �ִ�.
/// �׷��ٸ�.. �� mesh�� boen����.. �� mesh�� skin���� ��°��ΰ�?
/// </summary>
std::shared_ptr<FBXMeshInfo> FBXParser::LoadMeshInfo(aiNode* node, aiMesh* mesh, const aiScene* scene)
{
    std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

    size_t vertexNum = mesh->mNumVertices;           // ���� �޽��� ���ؽ� �� ����
        
    fbxMeshInfo->meshName = mesh->mName.C_Str();     // ���� �޽ÿ� �̸��� �ִ´�.

    fbxMeshInfo->nodeTM = ConvertMatrix(node->mTransformation);
    
    // ���ؽ��� �ִ´�.
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

	/*if(mesh->HasBones())
		ExtractBoneWeight(mesh, scene, fbxMeshInfo);*/

    //LoadAnimation(scene);

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

        // Baser Color �ؽ��Ķ�� �̸������ִ� �͵��� �� �̳����� �ҷ������°���?
        // �ٵ� �� �Ѵ� ������� �Ǽ� ���������
        // Diffuse Texture
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString texturePath;

			// �ؽ��� ������ ��ġ(�̸� ����)�� ���� �� �ִ�. 
			material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

			// �ٵ� ���� �ؽ��� ��ġ ������ �ʿ���⿡.. �̸��� �����ðž�
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

        // ���� ���͸��� ����Ʈ�� Ǫ��
        fbxModel->materialList.emplace_back(fbxMaterialInfo);
    }
}

/// <summary>
/// �ش� Mesh�� Bone��� Vertex�� �����ؾ��Ѵ�.
/// �׷��鼭 vertex�� �ش��ϴ� ����ġ�� �־��ش�.
/// aiMesh�� bone�迭�� vertex�迭�� ������ �ִ�.
/// [����!] 
/// Assimp�� �Ἥ �ε��ϸ� Bone�� Mesh ��� ���Ѵ� (bone �� GeomObject ����ϴ� ASE�� �ٸ���)
/// </summary>
void FBXParser::ExtractBoneWeight(aiMesh* mesh, const aiScene* scene, std::shared_ptr<FBXMeshInfo>& fbxMeshInfo)
{
    int boneNum = mesh->mNumBones;                           // �� �޽ÿ� ���Ե� bone�� ��
    
    for (int boneCnt = 0; boneCnt < boneNum; boneCnt++)
    {
        int boneID = -1;

        auto vertex = mesh->mBones[boneCnt]->mWeights;       // �� bone�� ���� ������ �޴� vertex��

        int vertexNum = mesh->mBones[boneCnt]->mNumWeights;  // �� bone�� ������ �޴� vertex ��

        // BoneMap�� Bone ������ �߰����� �ʾҴٸ�
        if (boneMap.find(mesh->mBones[boneCnt]->mName.C_Str()) == boneMap.end())
        {
            boneID = boneCounter;

		    std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

		    fbxBoneInfo->boneID = boneCounter;                              // �� id

		    fbxBoneInfo->boneName = mesh->mBones[boneCnt]->mName.C_Str();   // ���� �̸�

		    fbxBoneInfo->offsetMatrix = ConvertMatrix(mesh->mBones[boneCnt]->mOffsetMatrix);

            // BoneMap�� Bone ���� �߰�
            boneMap.insert(std::make_pair(fbxBoneInfo->boneName, fbxBoneInfo));
            
            // FBXModel Skeleton�� Bone ����  �߰�
            fbxModel->fbxSkeletonInfo->fbxBoneInfoList.emplace_back(fbxBoneInfo);

            boneCounter++;
        }
        // Bone ������ �̹� �ִٸ�
        else
        {
            boneID = boneMap[mesh->mBones[boneCnt]->mName.C_Str()]->boneID;
        }

        // �ش� bone�� ������ �޴� vertex���� ���ž�
        // �ش� �޽ð� bone�� ������ �޴´ٸ� for���� ���� �ƴϸ� �ȵ���.
        // ���ٸ� ��Ű�� �޽��̴�.
        for (int vertexCnt = 0; vertexCnt < vertexNum; vertexCnt++)
        {
			fbxModel->isSkinnedAnimation = true;            // ��·�� ��Ű�� �ִϸ��̼��� �ִٰ�����

			fbxMeshInfo->isSkinned = true;                  // �� �޽ô� ��Ų�� �޽��̴�.

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
    }
}

/// <summary>
/// Rotation�� ���ʹϾ� ���� ���� �������� ������ ���� �����ϹǷ� �״�� ����ϸ� �ȴ�.
/// ���ʹϾ� ���� ������ �׷��Ƚ� ���信 ���� wxyz���̹Ƿ� DX�� ����� �� ����
/// �ִϸ��̼� ���� ���� ����� ����� ���̹Ƿ� ���� ����� ���鶧 ��������� ����� �ʿ� ����.
/// ����� ��忡���� Ű���� ��� ������ ä�� pos, scale, rot ��� Ű ���� �����ӿ� ���� 1:1 ��Ī�Ǿ� ��������
/// ������ ��忡���� Ű ���� ���� ���� �߰����� �ʾ� ���� �ٸ��� ���� �� �ִ�.
/// </summary>
void FBXParser::LoadAnimation(const aiScene* scene)
{
    // �ִϸ��̼��� ���� ��ŭ
    for (int i = 0; i < scene->mNumAnimations; i++) 
    {
        auto aiAni = scene->mAnimations[i];

        float lastTime = 0.f;
        
        std::shared_ptr<FBXAnimClipInfo> aniInfo = std::make_shared<FBXAnimClipInfo>();
        aniInfo->duration = ((float)aiAni->mDuration);
        aniInfo->ticksPerFrame = ((float)aiAni->mTicksPerSecond);

        std::string aniName = aiAni->mName.C_Str();

        // ����� ��� ��ŭ...
        for (UINT j = 0; j < aiAni->mNumChannels; j++) 
        {
            auto aiAniNode = aiAni->mChannels[j];

            std::shared_ptr<FBXKeyFrameInfo> aniKeyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

            // pos, rot, scale key�߿� ���� ū�ɷ�ٰ� ����
            UINT keyCnt = std::max(aiAniNode->mNumPositionKeys, aiAniNode->mNumRotationKeys);
            keyCnt = std::max(keyCnt, aiAniNode->mNumScalingKeys);

            // Ű ������ ������ �����Ѵ�.
            DirectX::SimpleMath::Vector3 translation = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
            DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
            DirectX::SimpleMath::Vector4 rotation = DirectX::SimpleMath::Vector4(0.f, 0.f, 0.f, 0.f);
            float	 time = 0.f;

            // Ű ���� ���� ���� ������, �������� �ִ�.
            // ���� ��� ���� ������ ä���.
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

        // ���� �ִϸ��̼� ����Ʈ�� �߰�
        fbxModel->animationClipList.emplace_back(aniInfo);
    }
}

void FBXParser::Release()
{
    
}
