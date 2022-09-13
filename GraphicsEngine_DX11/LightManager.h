#pragma once
#include "ConstantBufferDefine.h"
#include "Graphics_PerFrameData.h"

enum class LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

class LightManager
{
public:
	LightManager() {};
	~LightManager() {};

private:
	static LightManager* lightManager;

public:
	static LightManager* Get();

public:
	// �̸�, ������ ����
	static std::map<std::string, LightInfo*> dirLights;
	static std::map<std::string, LightInfo*> pointLights;
	static std::map<std::string, LightInfo*> spotLights;

	// �������� �� light constantBuffer
	static cbLight		cbLightBuffer;

public:
	void Initialize();

	void SetLightInfo(LightInfo* lightInfo);

	void UpdateLightInfo(LightInfo* lightInfo);

	void ResetLightInfo();

	void Update();
};



