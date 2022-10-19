#pragma once
#include "ParserBase/ParserBase.h"

struct FBXMeshInfo;

/// <summary>
/// 나중에는 자체포맷 파싱만을 목적으로 하는 어플리케이션으로 하나 더 제작 할 것
/// 일단 이것은 FBXLoad 테스트용 dll, binarySerializer후 테스트용 으로 쓰자
/// 해야할 일
/// 1. FBXLoad Static Mesh 가 제대로 파싱이 되는지
/// 2. FBXLoad Skinned Mesh가 제대로 파싱이 되는지 (Animation이 제대로 되는지 확인 할 것)
/// 3. BinarySerialization 으로 저장 후 불러와서 제대로 파싱이 되는지 확인
/// 4. FBXFile -> FBXLoad -> BinarySerialzation 시키는 어플리케이션 제작할 것
///	   [정해진 경로에서 FBX를 가져온다 -> Binary화 해서 bin 폴더에 저장한다]
/// </summary>
class FBXParser : public ParserBase
{
private:

	std::shared_ptr<FBXModel> fbxModel;			// 모델

	fbxsdk::FbxManager* manager = nullptr;
	fbxsdk::FbxScene* scene = nullptr;
	fbxsdk::FbxImporter* importer = nullptr;

public:
	FBXParser();
	~FBXParser();

public:
	std::shared_ptr<FBXModel> LoadFbx(const std::string& path) override;

	void ParseNode(fbxsdk::FbxNode* node);

	void Import(const std::string& path);

	void LoadMesh(fbxsdk::FbxMesh* mesh);

	void LoadBones(fbxsdk::FbxNode* node, int idx, int parentIdx);
	
	void LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial);

	void GetNormal(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int vertexCounter);
	
	void GetTangent(std::shared_ptr<FBXMeshInfo>& meshInfo);
	
	void GetUV(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int uvIndex);

	std::wstring GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty);
};

/// <summary>
///	assimp에서 꺼내는 aiMatrix4x4는 모두 전치가 필요
/// </summary>
//inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(aiMatrix4x4 aimatrix)
//{
//	return DirectX::SimpleMath::Matrix
//	(
//		aimatrix.a1, aimatrix.b1, aimatrix.c1, aimatrix.d1,
//		aimatrix.a2, aimatrix.b2, aimatrix.c2, aimatrix.d2,
//		aimatrix.a3, aimatrix.b3, aimatrix.c3, aimatrix.d3,
//		aimatrix.a4, aimatrix.b4, aimatrix.c4, aimatrix.d4
//	);
//}