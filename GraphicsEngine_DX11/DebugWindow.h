#pragma once

/// <summary>
/// 2D 디버그용 윈도우의 개념으로 일반적인 비트맵 이미지가 아닌 RTT 텍스쳐에 그리는것이다.
/// 다중처리된 효과를 디버그 할때 이걸로 화면에 띄우면 된다.
/// </summary>
class DebugWindow
{
public:
	DebugWindow();
	~DebugWindow();

	bool Initialize(ComPtr<ID3D11Device> device);

	void Release();

	bool Render(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 pos);

	int GetIndexCount() { return _indexCount; }

private:
	bool InitializeBuffers(ComPtr<ID3D11Device> device);

	bool UpdateBuffers(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 pos);

	void RenderBuffers(ComPtr<ID3D11DeviceContext> deviceContext);

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	int _vertexCount;
	int _indexCount;
	int _screenWidth;
	int _screenHeight;

	int _textureWidth;
	int _textureHeight;

	Vector4 _previousPos;
};

