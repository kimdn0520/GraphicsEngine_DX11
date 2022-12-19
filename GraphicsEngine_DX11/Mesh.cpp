#include "pch.h"
#include "Mesh.h"
#include "Device.h"

void Mesh::CreateIndexBuffer(const std::vector<unsigned int>& indices)
{
	//---------------------------------------------------------------------------------------------------------
	// �ε��� ���۸� ����� ����
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

	indexBuffersSize.emplace_back(indices.size());

	ComPtr<ID3D11Buffer> indexBuffer;

	Graphics_Interface::Get()->GetDeviceClass()->GetDevice()->
		CreateBuffer(&indexBufferDesc, &indexInitData, indexBuffer.GetAddressOf());

	indexBuffers.emplace_back(indexBuffer);
}
