#pragma once
#include "ParserBase/ParserBase.h"

struct FBXMeshInfo;

/// <summary>
/// ���߿��� �Ľ̸��� �������� �ϴ� ���ø����̼����� �ϳ� �� ���� �� ��
/// �ϴ� �̰��� FBXLoad �׽�Ʈ�� dll, binarySerializer�� �׽�Ʈ�� ���� ����
/// �ؾ��� ��
/// 1. FBXLoad Static Mesh �� ����� �Ľ��� �Ǵ��� (���ؽ� �ɰ���, �ߺ� ���� �� ��)
/// 2. FBXLoad Skinned Mesh�� ����� �Ľ��� �Ǵ��� (Animation�� ����� �Ǵ��� Ȯ�� �� ��)
/// 3. BinarySerialization ���� ���� �� �ҷ��ͼ� ����� �Ľ��� �Ǵ��� Ȯ��
/// 4. FBXFile -> FBXLoad -> BinarySerialzation ��Ű�� ���ø����̼� ������ ��
///	   [������ ��ο��� FBX�� �����´� -> Binaryȭ �ؼ� bin ������ �����Ѵ�]
/// </summary>
class FBXParser : public ParserBase
{
private:
	Assimp::Importer importer;

	const aiScene* scene;

	std::shared_ptr<FBXModel> fbxModel;		// ��

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