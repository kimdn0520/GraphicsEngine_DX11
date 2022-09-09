#pragma once
#include "Graphics_Interface.h"

namespace GraphicsEngine
{
	class Mesh
	{
	public:
		ComPtr<ID3D11Buffer> vertexBuffer;

		ComPtr<ID3D11Buffer> indexBuffer;

		D3D11_PRIMITIVE_TOPOLOGY topology;

		unsigned int indexBufferSize;

		unsigned int stride = 0;
	
	public:
		template <typename T>
		void CreateVertexBuffer(const std::vector<T>& vertices);

		void CreateIndexBuffer(const std::vector<unsigned int>& indexBuffer);
	};

	template<typename T>
	inline void Mesh::CreateVertexBuffer(const std::vector<T>& vertices)
	{
		//---------------------------------------------------------------------------------------------------------
		// 정점 버퍼를 만드는 과정
		//---------------------------------------------------------------------------------------------------------
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = sizeof(T) * vertices.size();
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = vertices.data();

		Graphics_Interface::Get()->GetDevice->
			CreateBuffer(&vertexBufferDesc, &vertexInitData, vertexBuffer.GetAddressOf());
	}
}


