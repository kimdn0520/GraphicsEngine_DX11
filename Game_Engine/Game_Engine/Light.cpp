#include "pch.h"
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsManager.h"

Light::Light(std::shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::LIGHT),
	_transform(gameObject->GetTransform())
{
	_lightInfo = std::make_shared<LightInfo>();

	_centerPos = Vector3(10, -10, -5);
	_shadowRadius = sqrtf(10.0f * 10.0f + 15.0f * 15.0f) * 4 + 15.0f;
}

Light::~Light()
{}

void Light::SetLightDirection(const Vector3& direction)
{
	_lightInfo->direction = direction;
	_curDirection = direction;
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo->lightType = static_cast<int>(type);

	_type = type;
}

/// <summary>
/// 정보들을 다 입력하고 LightInfo를 그래픽스쪽에 보내서 등록한다.
/// </summary>
void Light::SendLightInfo()
{
	_lightInfo->name = GetGameObject()->GetName();

	GraphicsManager::Get()->SendLightData(_lightInfo);
}

void Light::Update()
{
	switch (_type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		_lightInfo->direction = _curDirection;
	}
	break;
	case LIGHT_TYPE::POINT_LIGHT:
	{
		_lightInfo->position = _transform->GetWorldPosition();
	}
	break;
	case LIGHT_TYPE::SPOT_LIGHT:
	{
		SetLightDirection(_transform->GetLook());
		_lightInfo->position = _transform->GetWorldPosition();
	}
	break;
	}

	// LightViewProj 재설정
	/*if (_curDirection != _preDirection)
	{
		SetLightViewProj();
	}*/

	_preDirection = _curDirection;

	GraphicsManager::Get()->UpdateLightData(_lightInfo);
}

void Light::SetLightViewProj()
{
	Vector3 dir;

	switch (_type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		dir = _curDirection;
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		dir = Vector3(1.0f, 1.0f, 1.0f);
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		dir = _curDirection;
		break;
	default:
		return;
	}

	Matrix lightView;
	Matrix lightProj;

	//Vector3 lightPos = -2.0f * _shadowRadius * dir;

	//lightView = XMMatrixLookAtLH(lightPos, _centerPos, Vector4(0.f, 1.0f, 0.0f, 0.0f));

	//// 광원 공간으로 변환
	//Vector3 lightSpace = XMVector3TransformCoord(_centerPos, lightView);

	//// 장면을 감싸는 광원 공간 직교 투영 상자
	//float l = lightSpace.x - _shadowRadius;
	//float b = lightSpace.y - _shadowRadius;
	//float n = lightSpace.z - _shadowRadius;
	//float r = lightSpace.x + _shadowRadius;
	//float t = lightSpace.y + _shadowRadius;
	//float f = lightSpace.z + _shadowRadius;

	// Light Proj
	//lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// 카메라 위치를 directional light로 해서 해보자!
	//lightView = XMMatrixLookAtLH( , ,Vector4(0.f, 1.0f, 0.0f, 0.0f));
	//lightProj = XMMatrixOrthographicLH(100 , 100, 0, 20);

	switch (_type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		_lightInfo->lightViewProj = lightView * lightProj;
	}
	break;
	case LIGHT_TYPE::POINT_LIGHT:
	{
		_lightInfo->lightViewProj = lightView * lightProj;
	}
	break;
	case LIGHT_TYPE::SPOT_LIGHT:
	{
		SetLightDirection(_transform->GetLook());
		_lightInfo->lightViewProj = lightView * lightProj;
	}
	break;
	}
}