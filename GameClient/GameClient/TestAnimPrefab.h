#pragma once
#include "IPrefab.h"

class TestAnimPrefab : public IPrefab
{
public:
	TestAnimPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~TestAnimPrefab();
};

