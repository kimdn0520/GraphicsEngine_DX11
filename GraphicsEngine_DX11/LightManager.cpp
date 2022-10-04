#include "pch.h"
#include "LightManager.h"

std::shared_ptr<LightManager> LightManager::lightManager = nullptr;
std::map<std::string, std::shared_ptr<LightInfo>> LightManager::dirLights;
std::map<std::string, std::shared_ptr<LightInfo>> LightManager::pointLights;
std::map<std::string, std::shared_ptr<LightInfo>> LightManager::spotLights;
cbLight LightManager::cbLightBuffer;

std::shared_ptr<LightManager> LightManager::Get()
{
	if (lightManager == nullptr)
		lightManager = std::make_shared<LightManager>();

	return lightManager;
}

void LightManager::Initialize()
{

}

void LightManager::SetLightInfo(std::shared_ptr<LightInfo> lightInfo)
{
	switch (lightInfo->lightType)
	{
	case (int)LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		dirLights.insert(make_pair(lightInfo->name, lightInfo));
	}
	break;
	case (int)LIGHT_TYPE::POINT_LIGHT:
	{
		pointLights.insert(make_pair(lightInfo->name, lightInfo));
	}
	break;
	case (int)LIGHT_TYPE::SPOT_LIGHT:
	{
		spotLights.insert(make_pair(lightInfo->name, lightInfo));
	}
	break;
	default:
		break;
	}
}

void LightManager::UpdateLightInfo(std::shared_ptr<LightInfo> lightInfo)
{
	switch (lightInfo->lightType)
	{
	case (int)LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		dirLights[lightInfo->name]->direction = lightInfo->direction;
	}
	break;
	case (int)LIGHT_TYPE::POINT_LIGHT:
	{
		pointLights[lightInfo->name]->position = lightInfo->position;
	}
	break;
	case (int)LIGHT_TYPE::SPOT_LIGHT:
	{
		spotLights[lightInfo->name]->direction = lightInfo->direction;
		spotLights[lightInfo->name]->position = lightInfo->position;
	}
	break;
	default:
		break;
	}
}

void LightManager::ResetLightInfo()
{
	dirLights.clear();
	pointLights.clear();
	spotLights.clear();
}

void LightManager::Update()
{
	cbLightBuffer.gDirLightCnt = dirLights.size();
	cbLightBuffer.gPointLightCnt = pointLights.size();
	cbLightBuffer.gSpotLightCnt = spotLights.size();
	int i = 0;

	for (auto it = dirLights.begin(); it != dirLights.end(); ++it)
	{
		cbLightBuffer.gDirLight[i].color = it->second->color;
		cbLightBuffer.gDirLight[i].power = it->second->power;
		cbLightBuffer.gDirLight[i].direction = it->second->direction;
	}
	i = 0;
	for (auto it = pointLights.begin(); it != pointLights.end(); ++it, i++)
	{
		cbLightBuffer.gPointLight[i].color = it->second->color;
		cbLightBuffer.gPointLight[i].power = it->second->power;
		cbLightBuffer.gPointLight[i].position = it->second->position;
		cbLightBuffer.gPointLight[i].range = it->second->range;
	}
	i = 0;
	for (auto it = spotLights.begin(); it != spotLights.end(); ++it, i++)
	{
		cbLightBuffer.gSpotLight[i].color = it->second->color;
		cbLightBuffer.gSpotLight[i].power = it->second->power;
		cbLightBuffer.gSpotLight[i].halfAngle = it->second->halfAngle;
		cbLightBuffer.gSpotLight[i].position = it->second->position;
		cbLightBuffer.gSpotLight[i].direction = it->second->direction;
		cbLightBuffer.gSpotLight[i].range = it->second->range;
	}
}


