#pragma once
#include "AlignedAllocationPolicy.h"
#include "GameEngineDLL.h"
#include "Graphics_PerFrameData.h"
#include "Component.h"

/// <summary>
/// Ambient : 광원이 방출하는 주변광의 양
/// Diffuse : 광원이 방출하는 분산광의 양(확산광)
/// Specular : 광원이 방출하는 반영광의 양(반사광)
/// Direction : 빛의 방향
/// Position : 광원의 위치
/// Range : 빛의 범위, 광원과의 거리가 이 범위보다 큰 점은 빛을 받지 않음
/// Attenuation : 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 세 개의
///				  감쇠 상수들을(a0, a1, a2)의 형태로 저장한다.
/// Spot : 점적광 계산에서 점적광 원뿔을 제어하는데 쓰이는 지수
/// Pad : 필요하다면 빛들의 배열을 설정할 수 있도록 마지막에 float 하나의 자리를 하나 마련해 둔다.
/// </summary>

class Transform;

enum class LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

class Light : public Component, public AlignedAllocationPolicy<16>
{
public:
	GameEngine_DLL Light(std::shared_ptr<GameObject> gameObject);
	virtual ~Light();

private:
	LIGHT_TYPE _type = LIGHT_TYPE::DIRECTIONAL_LIGHT;

	shared_ptr<LightInfo> _lightInfo;

	shared_ptr<Transform> _transform;

	Vector3 _preDirection;

	Vector3 _curDirection;

	Vector3 _centerPos;

	float _shadowRadius;

public:
	GameEngine_DLL void SetLightType(LIGHT_TYPE type);

	GameEngine_DLL void SetLightDirection(const Vector3& direction);

	GameEngine_DLL void SetColor(const Vector3& color) { _lightInfo->color = color; }
	GameEngine_DLL void SetPower(float power) { _lightInfo->power = power; }
	GameEngine_DLL void SetRange(float range) { _lightInfo->range = range; }
	GameEngine_DLL void SetHalfAngle(float halfAngle) { _lightInfo->halfAngle = halfAngle; }

	void SetLightViewProj();

	GameEngine_DLL void SendLightInfo();

	void Update() override;
};