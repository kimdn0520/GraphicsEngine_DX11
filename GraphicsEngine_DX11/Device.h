#pragma once
#include "WindowInfo.h"

class Device
{
private:
	// COM(Component Object Model)
	// - DX�� ���α׷��� ��� �������� ���� ȣȯ���� �����ϰ� �ϴ� ���
	// - COM ��ü(COM �������̽�)�� ���. ���λ����� �츮���� ������
	// - ComPtr ������ ����Ʈ ������
	ComPtr<ID3D11Device>	_device;			// ���� ��ü ����

	ComPtr<ID3D11DeviceContext> _deviceContext;

public:
	void Initialize(WindowInfo& info);

	void Release();

	const ComPtr<ID3D11Device>& GetDevice() { return _device; }
		
	const ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return _deviceContext; }
};

