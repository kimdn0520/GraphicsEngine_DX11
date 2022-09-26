#pragma once
#include "IPrefab.h"

class GenjiPrefab : public IPrefab 
{
public:
	GenjiPrefab(Vector3 pos = {0.f, 0.f, 0.f });
	~GenjiPrefab();
};

