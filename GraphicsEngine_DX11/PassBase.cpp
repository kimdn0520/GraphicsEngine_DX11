#include "pch.h"
#include "PassBase.h"
#include "Graphics_Interface.h"
#include "Device.h"

Microsoft::WRL::ComPtr<ID3D11Device> PassBase::g_device = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> PassBase::g_deviceContext = nullptr;

void PassBase::Initialize()
{
	g_device = Graphics_Interface::Get()->GetDeviceClass()->GetDevice();
	
	g_deviceContext = Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext();
}

void PassBase::Reset()
{
	g_device.ReleaseAndGetAddressOf();

	g_deviceContext.ReleaseAndGetAddressOf();
}
