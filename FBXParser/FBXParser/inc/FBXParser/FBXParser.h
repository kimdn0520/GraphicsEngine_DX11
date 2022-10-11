#pragma once
#include "ParserBase/ParserBase.h"

struct FBXMeshInfo;

/// <summary>
/// 나중에는 파싱만을 목적으로 하는 어플리케이션으로 하나 더 제작 할 것
/// 일단 이것은 FBXLoad 테스트용 dll, binarySerializer후 테스트용 으로 쓰자
/// 해야할 일
/// 1. FBXLoad Static Mesh 가 제대로 파싱이 되는지 (버텍스 쪼개기, 중복 제거 할 것)
/// 2. FBXLoad Skinned Mesh가 제대로 파싱이 되는지 (Animation이 제대로 되는지 확인 할 것)
/// 3. BinarySerialization 으로 저장 후 불러와서 제대로 파싱이 되는지 확인
/// 4. FBXFile -> FBXLoad -> BinarySerialzation 시키는 어플리케이션 제작할 것
///	   [정해진 경로에서 FBX를 가져온다 -> Binary화 해서 bin 폴더에 저장한다]
/// </summary>
class FBXParser : public ParserBase
{
private:
	Assimp::Importer importer;

	const aiScene* scene;

	std::shared_ptr<FBXModel> fbxModel;		// 모델

	int boneID = 0;

public:
	FBXParser();
	~FBXParser();

public:
	std::shared_ptr<FBXModel> LoadFbx(const std::string& path) override;

	void ParseNode(aiNode* node, const aiScene* scene);

	std::shared_ptr<FBXMeshInfo> LoadMeshInfo(aiMesh* mesh, const aiScene* scene);

	void LoadMaterial(const aiScene* scene);

	void ExtractBoneWeight(aiMesh* mesh, const aiScene* scene, std::shared_ptr<FBXMeshInfo>& fbxMeshInfo);

	void LoadAnimation(const aiScene* scene);

	DirectX::SimpleMath::Matrix ConvertMatrix(aiMatrix4x4 aimatrix);

	void Release() override;
};

inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(aiMatrix4x4 aimatrix)
{
	return DirectX::SimpleMath::Matrix
	(
		aimatrix.a1, aimatrix.a2, aimatrix.a3, aimatrix.a4,
		aimatrix.b1, aimatrix.b2, aimatrix.b3, aimatrix.b4,
		aimatrix.c1, aimatrix.c2, aimatrix.c3, aimatrix.c4,
		aimatrix.d1, aimatrix.d2, aimatrix.d3, aimatrix.d4
	);
}