#include "pch.h"
#include "DataManager.h"
#include "Resources.h"

std::shared_ptr<DataManager> DataManager::dataManager = nullptr;

std::shared_ptr<DataManager> DataManager::Get()
{
	if (dataManager == nullptr)
		dataManager = make_shared<DataManager>();

	return dataManager;
}

void DataManager::DataLoad()
{
	isDataLoad = true;

	_dataResources.insert(std::make_pair("Genji", Resources::Get()->LoadASE("Data/ASE/genji_max.ase", TOPOLOGY_TRIANGLELIST, RasterState_SOLID)));

	_dataResources.insert(std::make_pair("Eye", Resources::Get()->LoadFBX("Data/FBX/eyeball.fbx", TOPOLOGY_TRIANGLELIST, RasterState_SOLID)));
}

void DataManager::Release()
{
	/*for (auto data : _dataResources)
	{
		for (auto gameObj : data.second)
		{
			delete gameObj;
		}
	}*/

	_dataResources.clear();
}

std::vector<std::shared_ptr<GameObject>> DataManager::GetDataResources(std::string data)
{
	return _dataResources[data];
}

