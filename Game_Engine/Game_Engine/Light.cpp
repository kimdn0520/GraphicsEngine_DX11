#include "pch.h"
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsManager.h"

Light::Light(GameObject* gameObject)
	: Component(gameObject, COMPONENT_TYPE::LIGHT),
	_transform(gameObject->GetTransform())
{
	_lightInfo = new LightInfo();
}

Light::~Light()
{}

void Light::SetLightDirection(const Vector3& direction)
{
	_lightInfo->direction = direction;
	_direction = direction;
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
		_lightInfo->direction = _direction;
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

	GraphicsManager::Get()->UpdateLightData(_lightInfo);
}
