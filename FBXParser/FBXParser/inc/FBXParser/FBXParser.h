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
	fbxsdk::FbxGeometryConverter* geometryConverter;

	FbxArray<FbxString*> animNames;

public:
	FBXParser();
	~FBXParser();

public:
	std::shared_ptr<FBXModel> LoadFbx(const std::string& path) override;

	void Import(const std::string& path);

	void ProcessMesh(fbxsdk::FbxNode* node, FbxNodeAttribute::EType attribute);

	void LoadMesh(fbxsdk::FbxNode* node, fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshData, int meshCnt);

	void ProcessBones(fbxsdk::FbxNode* node, int idx, int parentIdx);
	
	void LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, std::shared_ptr<FBXMeshInfo>& meshData);

	void LoadAnimation();

	void ProcessAnimationData(FbxNode* node);

	int FindBoneIndex(std::string boneName);

	DirectX::SimpleMath::Matrix GetNodeTM(fbxsdk::FbxNode* node);
	
	void GetNormal(fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshInfo, int idx, int vertexCounter);
	
	void GetTangent(std::shared_ptr<FBXMeshInfo>& meshInfo, int meshCnt);

	DirectX::SimpleMath::Vector2 GetUV(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	std::wstring GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty);

	fbxsdk::FbxAMatrix GetTransformMatrix(FbxNode* node);

	DirectX::SimpleMath::Vector4 ConvertVector4(fbxsdk::FbxVector4 v4);

	DirectX::SimpleMath::Vector4 ConvertAniVector4(fbxsdk::FbxVector4 v4);

	DirectX::SimpleMath::Matrix ConvertMatrix(fbxsdk::FbxMatrix matrix);

	DirectX::SimpleMath::Matrix ConvertAniMatrix(fbxsdk::FbxMatrix matrix);
};

inline DirectX::SimpleMath::Vector4 FBXParser::ConvertVector4(fbxsdk::FbxVector4 v4)
{
	// xyzw -> xzyw
	return DirectX::SimpleMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3])
	);
}

inline DirectX::SimpleMath::Vector4 FBXParser::ConvertAniVector4(fbxsdk::FbxVector4 v4)
{
	return DirectX::SimpleMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[3])
	);
}

inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	return DirectX::SimpleMath::Matrix
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}

inline DirectX::SimpleMath::Matrix FBXParser::ConvertAniMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	return DirectX::SimpleMath::Matrix
	(
		ConvertAniVector4(r1),
		ConvertAniVector4(r2),
		ConvertAniVector4(r3),
		ConvertAniVector4(r4)
	);
}