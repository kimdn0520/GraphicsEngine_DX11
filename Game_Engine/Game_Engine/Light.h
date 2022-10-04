#pragma once
#include "AlignedAllocationPolicy.h"
#include "GameEngineDLL.h"
#include "Graphics_PerFrameData.h"
#include "Component.h"

/// <summary>
/// Ambient : ������ �����ϴ� �ֺ����� ��
/// Diffuse : ������ �����ϴ� �л걤�� ��(Ȯ�걤)
/// Specular : ������ �����ϴ� �ݿ����� ��(�ݻ籤)
/// Direction : ���� ����
/// Position : ������ ��ġ
/// Range : ���� ����, �������� �Ÿ��� �� �������� ū ���� ���� ���� ����
/// Attenuation : ���� ���Ⱑ �Ÿ��� ���� �����ϴ� ����� �����ϴ� �� ����
///				  ���� �������(a0, a1, a2)�� ���·� �����Ѵ�.
/// Spot : ������ ��꿡�� ������ ������ �����ϴµ� ���̴� ����
/// Pad : �ʿ��ϴٸ� ������ �迭�� ������ �� �ֵ��� �������� float �ϳ��� �ڸ��� �ϳ� ������ �д�.
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