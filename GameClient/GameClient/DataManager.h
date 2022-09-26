#pragma once

class GameObject;

class DataManager
{
public:
	DataManager() {};
	~DataManager() {};

private:
	static std::shared_ptr<DataManager> dataManager;

public:
	static std::shared_ptr<DataManager> Get();

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> _dataResources;

public:
	bool isDataLoad = false;

public:
	void DataLoad();

	void Release();
	
	std::vector<std::shared_ptr<GameObject>> GetDataResources(std::string data);
};

