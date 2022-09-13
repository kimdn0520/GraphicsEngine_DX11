#include "pch.h"
#include "DataManager.h"
#include "Resources.h"

DataManager* DataManager::dataManager = nullptr;

DataManager* DataManager::Get()
{
	if (dataManager == nullptr)
		dataManager = new DataManager();

	return dataManager;
}

void DataManager::DataLoad()
{
	isDataLoad = true;

	_dataResources.insert(std::make_pair("Genji", Resources::Get()->LoadASE("Data/ASE/genji_max.ase", TOPOLOGY_TRIANGLELIST, RasterState_SOLID)));
}

void DataManager::Release()
{
	for (auto data : _dataResources)
	{
		delete& data.second;
	}

	_dataResources.clear();
}

std::vector<GameObject*> DataManager::GetDataResources(std::string data)
{
	return _dataResources[data];
}

