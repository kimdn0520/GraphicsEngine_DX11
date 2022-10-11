#pragma once
#include "IPrefab.h"

class TestFBXPrefab : public IPrefab
{
public:
	TestFBXPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~TestFBXPrefab();
};

