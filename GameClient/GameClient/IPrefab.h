#pragma once

class GameObject;

class IPrefab abstract
{
public:
	IPrefab(std::shared_ptr<GameObject> gameObj);
	~IPrefab() = default;

public:
	std::shared_ptr<GameObject> gameObject;
};

