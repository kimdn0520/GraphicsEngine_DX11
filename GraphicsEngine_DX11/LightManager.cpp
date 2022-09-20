#include "pch.h"
#include "LightManager.h"

LightManager* LightManager::lightManager = nullptr;
std::map<std::string, LightInfo*> LightManager::dirLights;
std::map<std::string, LightInfo*> LightManager::pointLights;
std::map<std::string, LightInfo*> LightManager::spotLights;
cbLight LightManager::cbLightBuffer;

LightManager* LightManager::Get()
{
	if (lightManager == nullptr)
		lightManager = new LightManager();

	return lightManager;
}

void LightManager::Initialize()
{

}

void LightManager::SetLightInfo(LightInfo* lightInfo)
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

void LightManager::UpdateLightInfo(LightInfo* lightInfo)
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
	for(auto dirLight : dirLights)
		delete dirLight.second;

	for (auto pointLight : pointLights)
		delete pointLight.second;

	for (auto spotLight : spotLights)
		delete spotLight.second;

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
		cbLightBuffer.gDirLight[i].Ambient = it->second->ambient;
		cbLightBuffer.gDirLight[i].Diffuse = it->second->diffuse;
		cbLightBuffer.gDirLight[i].Specular = it->second->specular;
		cbLightBuffer.gDirLight[i].Direction = it->second->direction;
		cbLightBuffer.gDirLight[i].LightViewProj = it->second->lightViewProj * _texSpace;
	}
	i = 0;
	for (auto it = pointLights.begin(); it != pointLights.end(); ++it, i++)
	{
		cbLightBuffer.gPointLight[i].Ambient = it->second->ambient;
		cbLightBuffer.gPointLight[i].Diffuse = it->second->diffuse;
		cbLightBuffer.gPointLight[i].Specular = it->second->specular;
		cbLightBuffer.gPointLight[i].Position = it->second->position;
		cbLightBuffer.gPointLight[i].Att = it->second->att;
		cbLightBuffer.gPointLight[i].Range = it->second->range;
		cbLightBuffer.gPointLight[i].LightViewProj = it->second->lightViewProj;
	}
	i = 0;
	for (auto it = spotLights.begin(); it != spotLights.end(); ++it, i++)
	{
		cbLightBuffer.gSpotLight[i].Ambient = it->second->ambient;
		cbLightBuffer.gSpotLight[i].Diffuse = it->second->diffuse;
		cbLightBuffer.gSpotLight[i].Specular = it->second->specular;
		cbLightBuffer.gSpotLight[i].Position = it->second->position;
		cbLightBuffer.gSpotLight[i].Direction = it->second->direction;
		cbLightBuffer.gSpotLight[i].Att = it->second->att;
		cbLightBuffer.gSpotLight[i].Range = it->second->range;
		cbLightBuffer.gSpotLight[i].Spot = it->second->spot;
		cbLightBuffer.gSpotLight[i].LightViewProj = it->second->lightViewProj;
	}
}


