#include "pch.h"
#include "Device.h"


void Device::Initialize(WindowInfo& info)
{
	D3D_FEATURE_LEVEL featureLevel;						// 피쳐 레벨
	UINT createDeviceFlags = 0;							// 플래그
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;		// 디버그 모드 빌드에서 디버그 계층을 활성화하는 플래그
#endif

		// device 생성
	HRESULT hResult = D3D11CreateDevice(
		0,												// 기본 어댑터 
		D3D_DRIVER_TYPE_HARDWARE,						// 3차원 그래픽 가속 적용
		0,												// 소프트웨어 장치를 사용하지 않음
		createDeviceFlags,								//  추가적인 장치 생성 플래그들
		0, 0,											// default feature level array - 널값을 지정하면 지원되는 최고 기능 수준이 선택된다.
		D3D11_SDK_VERSION,
		_device.GetAddressOf(),
		&featureLevel,
		_deviceContext.GetAddressOf());

	if (FAILED(hResult))
		return;

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		return;

	// 4X MSAA 품질 수준 지원 점검
	_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &info.MSAAQuality_4x);
	assert(info.MSAAQuality_4x > 0);					// 4X MSAA가 항상 지원되므로, 반환된 품질 수준 값은 반드시 0보다 커야 한다.
}

void Device::Release()
{
	_device.ReleaseAndGetAddressOf();
	_deviceContext.ReleaseAndGetAddressOf();
	delete this;
}


