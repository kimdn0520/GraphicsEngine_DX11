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
	// ASEParser 객체 생성
	_aseParser = new ASEParser();

	LoadCubeMesh(TOPOLOGY_TRIANGLELIST, RasterState_SOLID);
}

void Resources::Release()
{
	// TODO : 음 좀더 봐야함.. Release를 만들어둘까
	_aseParser->ResetData();

	delete _aseParser;
}

void Resources::LoadCubeMesh(int topology, int rasterState)
{

}

void Resources::LoadSphereMesh(int topology, int rasterState)
{

}

void Resources::LoadAxisMesh(int topology, int rasterState)
{

}

void Resources::LoadGridMesh(int topology, int rasterState)
{

}

void Resources::LoadScreenMesh(int topology, int rasterState)
{

}

