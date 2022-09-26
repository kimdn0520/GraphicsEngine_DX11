#include "pch.h"
#include "IPrefab.h"

IPrefab::IPrefab(std::shared_ptr<GameObject> gameObj)
{
	gameObject = gameObj;
}
