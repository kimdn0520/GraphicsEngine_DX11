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

	void Release() override;
};

