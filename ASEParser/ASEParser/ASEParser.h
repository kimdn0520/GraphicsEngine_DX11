#pragma once
#include "ASEParserDLL.h"
#include "SimpleMath.h"
#include <vector>
#include "ASEParserData.h"
using namespace DirectX;
using namespace SimpleMath;

class Lexer;

// 버텍스 쪼개기,중복제거 할때 쓰려고연
struct VertexInfo
{
	Vector2 uv;
	Vector3 normal;
	int index;
};

class ASEParser
{
public:
	ASEParser_DLL ASEParser();
	ASEParser_DLL ~ASEParser();

private:
	int _asciiexport;									// 맨 처음에 나오는 export값

	Lexer* _lexer;										// Lexer

	char _lineArray[256] = { 0 };						// 한 줄

	ASEData::ASEModel* _ASEModel;

	int _materialIndex = 0;
	int _materialNum = 0;								// MATERIAL_REF 에서 가져온다.
	int curTexture = 0;									// 파싱중 현재 텍스쳐 상태

	ASEData::ASEMeshInfo* _parsingMeshInfo;				// 현재 파싱중인 메시~
	ASEData::ASEAnimation* _parsingAnimation;			// 현재 파싱중인 애니메이션~
	Vector3 _row0;
	Vector3 _row1;
	Vector3 _row2;
	Vector3 _row3;
	bool _isNegativeScale = false;

	int _numVertex = 0;
	int _normalIdx = 0;
	std::vector<ASEData::Vertex> _vertices;
	vector<unsigned int> _indices;
	vector<ASEData::Vertex> _tmpVertices;
	vector<unsigned int> _tmpIndices;
	vector<Vector2> _tmpTVert;
	vector<unsigned int> _tmpTFaceIndices;
	vector<Vector3> _tmpNormal;

	int _weightNum = 0;
	int _weightCount = 0;

public:
	ASEParser_DLL ASEData::ASEModel* Load(string filePath);

	void Optimize();

	ASEParser_DLL void ResetData();

	void PushMesh();

	string Parsing_String();

	int Parsing_Int();

	float Parsing_Float();

	void DecomposeMatrix(XMMATRIX worldTM, XMFLOAT3& scale, XMFLOAT3& rotation, XMFLOAT3& trans);
};

