#pragma once

class GameObject;

class DataManager
{
public:
	DataManager() {};
	~DataManager() {};

private:
	static DataManager* dataManager;

public:
	static DataManager* Get();

private:
	std::unordered_map<std::string, std::vector<GameObject*>> _dataResources;

public:
	bool isDataLoad = false;

public:
	void DataLoad();

	void Release();
	
	std::vector<GameObject*> GetDataResources(std::string data);
};

