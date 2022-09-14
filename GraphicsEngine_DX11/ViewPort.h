#pragma once

class ViewPort
{
public:
	void Initialize(const Vector2& leftTop, const int& screenWidth, const int& screenHeight);

	void OnResize(const int& screenWidth, const int& screenHeight);

	void OnResize(const Vector2 leftTop, const int& screenWidth, const int& screenHeight);

	void SetViewPort(ComPtr<ID3D11DeviceContext> deviceContext);

	void Release();

private:
	Vector2 _leftTop;

	D3D11_VIEWPORT _viewport;
};

