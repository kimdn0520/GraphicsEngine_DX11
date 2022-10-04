#pragma once
#include "SimpleMath.h"
#include <string>

struct LightInfo
{
	int 			lightType;
	std::string		name;

	DirectX::SimpleMath::Vector3	color;
	float power;

	DirectX::SimpleMath::Vector3	position;
	DirectX::SimpleMath::Vector3	direction;

	DirectX::SimpleMath::Vector3 att;
	float		range;
	float		halfAngle;

	DirectX::SimpleMath::Matrix lightViewProj;
};

struct CameraInfo
{
	DirectX::SimpleMath::Vector3 worldPos;

	DirectX::SimpleMath::Matrix viewTM;
	DirectX::SimpleMath::Matrix projTM;
	DirectX::SimpleMath::Matrix orthoProjTM;
	DirectX::SimpleMath::Matrix reflectionTM;
};