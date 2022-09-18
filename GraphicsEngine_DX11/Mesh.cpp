#include "pch.h"
#include "Mesh.h"
#include "Device.h"

void Mesh::CreateIndexBuffer(const std::vector<unsigned int>& indices)
{
	//---------------------------------------------------------------------------------------------------------
	// 인덱스 버퍼를 만드는 과정
	//---------------------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = indices.data();

	_indexBufferSize = indices.size();

	Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
		CreateBuffer(&indexBufferDesc, &indexInitData, indexBuffer.GetAddressOf());
}
