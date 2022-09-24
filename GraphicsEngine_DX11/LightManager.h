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
	static std::shared_ptr<LightManager> lightManager;

public:
	static std::shared_ptr<LightManager> Get();

public:
	// �̸�, ������ ����
	static std::map<std::string, std::shared_ptr<LightInfo>> dirLights;
	static std::map<std::string, std::shared_ptr<LightInfo>> pointLights;
	static std::map<std::string, std::shared_ptr<LightInfo>> spotLights;

	// �������� �� light constantBuffer
	static cbLight		cbLightBuffer;

public:
	void Initialize();

	void SetLightInfo(std::shared_ptr<LightInfo> lightInfo);

	void UpdateLightInfo(std::shared_ptr<LightInfo> lightInfo);

	void ResetLightInfo();

	void Update();
};



