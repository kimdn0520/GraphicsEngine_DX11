#pragma once
#include "ParserBase/ParserBase.h"

struct FBXMeshInfo;
struct FBXBoneInfo;

struct NodeInfo 
{
	int				depth = 0;			

	std::string		nodeName = "none";
	std::shared_ptr<NodeInfo> parent = nullptr;

	NodeInfo(std::shared_ptr<NodeInfo> parent, std::string name, DirectX::SimpleMath::Matrix tm, int depth = 0)
		: parent(parent), nodeName(name), depth(depth)
	{}
};

/// <summary>
/// ���߿��� ��ü���� �Ľ̸��� �������� �ϴ� ���ø����̼����� �ϳ� �� ���� �� ��
/// �ϴ� �̰��� FBXLoad �׽�Ʈ�� dll, binarySerializer�� �׽�Ʈ�� ���� ����
/// �ؾ��� ��
/// 1. FBXLoad Static Mesh �� ����� �Ľ��� �Ǵ���
/// 2. FBXLoad Skinned Mesh�� ����� �Ľ��� �Ǵ��� (Animation�� ����� �Ǵ��� Ȯ�� �� ��)
/// 3. BinarySerialization ���� ���� �� �ҷ��ͼ� ����� �Ľ��� �Ǵ��� Ȯ��
/// 4. FBXFile -> FBXLoad -> BinarySerialzation ��Ű�� ���ø����̼� ������ ��
///	   [������ ��ο��� FBX�� �����´� -> Binaryȭ �ؼ� bin ������ �����Ѵ�]
/// </summary>
class FBXParser : public ParserBase
{
private:
	Assimp::Importer importer;

	std::shared_ptr<FBXModel> fbxModel;		// ��

	std::unordered_map<std::string, std::shared_ptr<FBXBoneInfo>> boneMap;

	int boneCounter = 0;

	std::unordered_map<std::string, std::shared_ptr<NodeInfo>> nodeInfoList;

public:
	FBXParser();
	~FBXParser();

public:
	std::shared_ptr<FBXModel> LoadFbx(const std::string& path) override;

	void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<NodeInfo> parent = nullptr, int depth = 0);

	std::shared_ptr<FBXMeshInfo> LoadMeshInfo(aiNode* node, aiMesh* mesh, const aiScene* scene);

	void LoadMaterial(const aiScene* scene);

	void ExtractBoneWeight(aiMesh* mesh, const aiScene* scene, std::shared_ptr<FBXMeshInfo>& fbxMeshInfo);

	void CalcBoneOffset(aiBone* bone, std::shared_ptr<FBXBoneInfo>& fbxBoneInfo);

	void LoadAnimation(const aiScene* scene);

	DirectX::SimpleMath::Matrix ConvertMatrix(aiMatrix4x4 aimatrix);

	void Release() override;
};

/// <summary>
///	assimp���� ������ aiMatrix4x4�� ��� ��ġ�� �ʿ�
/// </summary>
inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(aiMatrix4x4 aimatrix)
{
	return DirectX::SimpleMath::Matrix
	(
		aimatrix.a1, aimatrix.b1, aimatrix.c1, aimatrix.d1,
		aimatrix.a2, aimatrix.b2, aimatrix.c2, aimatrix.d2,
		aimatrix.a3, aimatrix.b3, aimatrix.c3, aimatrix.d3,
		aimatrix.a4, aimatrix.b4, aimatrix.c4, aimatrix.d4
	);
}