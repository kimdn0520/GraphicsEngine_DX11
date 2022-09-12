#include "pch.h"
#include "Resources.h"
#include "ASEParser.h"
#include "GraphicsManager.h"

Resources* Resources::resources = nullptr;

Resources* Resources::Get()
{
	if (resources == nullptr)
		resources = new Resources();

	return resources;
}

void Resources::Initialize()
{
	// ASEParser ∞¥√º ª˝º∫
	_aseParser = new ASEParser();

	LoadCubeMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);
}

void Resources::Release()
{
	// TODO : ¿Ω ¡ª¥ı ∫¡æﬂ«‘.. Release∏¶ ∏∏µÈæÓµ—±Ó
	_aseParser->ResetData();

	delete _aseParser;
}

void Resources::LoadCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<StaticMeshVertex> vertices(24);

	// æ’∏È
	vertices[0] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[1] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[2] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[3] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	// µﬁ∏È
	vertices[4] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[5] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[6] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[7] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	// ¿≠∏È
	vertices[8] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[9] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[10] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[11] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	// æ∆∑ß∏È
	vertices[12] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[13] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[14] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[15] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	// øﬁ¬ ∏È		
	vertices[16] = StaticMeshVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[17] = StaticMeshVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[18] = StaticMeshVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[19] = StaticMeshVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	// ø¿∏•¬ ∏È		
	vertices[20] = StaticMeshVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[21] = StaticMeshVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[22] = StaticMeshVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[23] = StaticMeshVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));

	vector<unsigned int> idx(36);

	// æ’∏È
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µﬁ∏È
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ¿≠∏È
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// æ∆∑ß∏È
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// øﬁ¬ ∏È
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ø¿∏•¬ ∏È
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	GraphicsManager::Get()->CreateMesh(
		vertices,
		idx,
		"CubeMesh",
		topology,
		rasterState);
}

void Resources::LoadSphereMesh(int topology, int rasterState)
{
	float radius = 0.5f;		// ±∏¿« π›¡ˆ∏ß
	int stackCount = 20;		// ∞°∑Œ ∫–«“
	int sliceCount = 20;		// ºº∑Œ ∫–«“

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	vector<StaticMeshVertex> vertices;

	// ∫œ±ÿ
	vertices.push_back(Vector3(0.0f, radius, 0.0f));

	// ∞Ì∏Æ∏∂¥Ÿ µπ∏Èº≠ ¡§¡°¿ª ∞ËªÍ«—¥Ÿ (∫œ±ÿ/≥≤±ÿ ¥‹¿œ¡°¿∫ ∞Ì∏Æ∞° X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ∞Ì∏Æø° ¿ßƒ°«— ¡§¡°
		for (int x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			Vector3 pos;
			pos.x = radius * sinf(phi) * cosf(theta);
			pos.y = radius * cosf(phi);
			pos.z = radius * sinf(phi) * sinf(theta);

			vertices.push_back(pos);
		}
	}

	// ≥≤±ÿ
	vertices.push_back(Vector3(0.0f, -radius, 0.0f));

	vector<unsigned int> idx;

	// ∫œ±ÿ ¿Œµ¶Ω∫
	for (int i = 0; i <= sliceCount; ++i)
	{
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ∏ˆ≈Î ¿Œµ¶Ω∫
	int ringVertexCount = sliceCount + 1;
	for (int y = 0; y < stackCount - 2; ++y)
	{
		for (int x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ≥≤±ÿ ¿Œµ¶Ω∫
	int bottomIndex = static_cast<int>(vertices.size()) - 1;
	int lastRingStartIndex = bottomIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	GraphicsManager::Get()->CreateMesh(
		vertices,
		idx,
		"SphereMesh",
		topology,
		rasterState);
}

void Resources::LoadAxisMesh(int topology, int rasterState)
{
	vector<StaticMeshVertex> vertices;
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(20.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 20.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(StaticMeshVertex(Vector3(0.0f, 0.0f, 20.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));

	vector<unsigned int> idx;
	idx.push_back(0); idx.push_back(1);
	idx.push_back(2); idx.push_back(3);
	idx.push_back(4); idx.push_back(5);

	GraphicsManager::Get()->CreateMesh(
		vertices,
		idx,
		"AxisMesh",
		topology,
		rasterState);
}

void Resources::LoadGridMesh(int topology, int rasterState)
{

}

void Resources::LoadScreenMesh(int topology, int rasterState)
{

}

