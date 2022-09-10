#pragma once

class Texture
{
public:
	Texture();
	~Texture();

private:
	ComPtr<ID3D11ShaderResourceView> _mapSRV;

	ScratchImage _image;

	ComPtr<ID3D11Texture2D> _texResource;

	DXGI_FORMAT _format;

	unsigned int _levels;

	unsigned int _width;

	unsigned int _height;

public:
	ComPtr<ID3D11ShaderResourceView> GetMapSRV() { return _mapSRV; }

	void Initialize(const std::wstring& path);

	void Release();
};



