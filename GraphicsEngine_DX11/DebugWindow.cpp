#include "pch.h"
#include "DebugWindow.h"
#include "VertexDefine.h"

DebugWindow::DebugWindow()
{
	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;
}

DebugWindow::~DebugWindow()
{
	_vertexBuffer.ReleaseAndGetAddressOf();
	_vertexBuffer = nullptr;

	_indexBuffer.ReleaseAndGetAddressOf();
	_indexBuffer = nullptr;
}

bool DebugWindow::Initialize(ComPtr<ID3D11Device> device)
{
	bool result;

	// 이전 위치를 음수로 초기화 한다.
	_previousPos = Vector4(-1.f, -1.f, -1.f, -1.f);

	result = InitializeBuffers(device);

	if (!result)
		return false;
	else
		return true;
}

void DebugWindow::Release()
{
	_vertexBuffer.ReleaseAndGetAddressOf();
	_indexBuffer.ReleaseAndGetAddressOf();
}

bool DebugWindow::Render(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 pos)
{
	bool result;

	result = UpdateBuffers(deviceContext, pos);

	if (!result)
		return false;

	RenderBuffers(deviceContext);

	return true;
}

bool DebugWindow::InitializeBuffers(ComPtr<ID3D11Device> device)
{
	// 정점이랑 인덱스 설정해줄거얌
	StaticMeshVertex* vertices;
	unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Set the number of vertices in the vertex array. 
	_vertexCount = 6;
	// Set the number of indices in the index array.
	_indexCount = _vertexCount;

	// Create the vertex array. 
	vertices = new StaticMeshVertex[_vertexCount];

	if (!vertices)
	{
		return false;
	}

	// Create the index array. 
	indices = new unsigned int[_indexCount];

	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first. 
	ZeroMemory(vertices, sizeof(StaticMeshVertex) * _vertexCount);

	// Load the index array with data. 
	// 구냥 0~5 꽂아버리눈듯?
	for (i = 0; i < _indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer. 
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(StaticMeshVertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer. 
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, _vertexBuffer.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer. 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data. 
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, _indexBuffer.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded. 
	delete[] vertices;
	delete[] indices;

	return true;
}

bool DebugWindow::UpdateBuffers(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 pos)
{
	// pos 는 left, right, top, bottom 순으로 넣을것 
	// left ~ right : -1.0f ~ 1.0f
	// top ~ bottom : 1.0f ~ -1.0f
	// 이 비트맵을 렌더링하는 위치가 변경되지 않은 경우 정점 버퍼를 업데이트 안해도 된다.
	if ((pos.x == _previousPos.x) && (pos.y == _previousPos.y) && (pos.z == _previousPos.z) && (pos.w == _previousPos.w))
		return true;

	// 위치가 변경이 되는 경우.. 위치 바까준담~
	_previousPos.x = pos.x;
	_previousPos.y = pos.y;
	_previousPos.z = pos.z;
	_previousPos.w = pos.w;

	float left, right, top, bottom;
	StaticMeshVertex* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	StaticMeshVertex* verticesPtr;
	HRESULT result;

	// left ~ right : -1.0f ~ 1.0f 군요..!
	// top ~ bottom : 1.0f ~ -1.0f 군요..? 왜지 감쟈
	left = pos.x;
	right = pos.y;
	top = pos.z;
	bottom = pos.w;

	vertices = new StaticMeshVertex[_vertexCount];
	if (!vertices)
		return false;

	// first triangle
	vertices[0].position = Vector3(left, top, 0.0f);				// Top Left
	vertices[0].uv = Vector2(0.0f, 0.0f);
	vertices[0].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[1].position = Vector3(right, bottom, 0.0f);			// Bottom Right
	vertices[1].uv = Vector2(1.0f, 1.0f);
	vertices[1].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].position = Vector3(left, bottom, 0.0f);				// Bottom Left
	vertices[2].uv = Vector2(0.0f, 1.0f);	
	vertices[2].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);							
	// second triangle
	vertices[3].position = Vector3(left, top, 0.0f);				// Top Left
	vertices[3].uv = Vector2(0.0f, 0.0f);
	vertices[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[4].position = Vector3(right, top, 0.0f);				// Top Right
	vertices[4].uv = Vector2(1.0f, 0.0f);
	vertices[4].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[5].position = Vector3(right, bottom, 0.0f);			// Bottom Right
	vertices[5].uv = Vector2(1.0f, 1.0f);
	vertices[5].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	result = deviceContext->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(result))
		return false;

	// 정점 버퍼의 데이터에 대한 포인터를 가져옵니다.
	verticesPtr = (StaticMeshVertex*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(StaticMeshVertex) * _vertexCount));

	deviceContext->Unmap(_vertexBuffer.Get(), 0);

	delete[] vertices;

	return true;
}

void DebugWindow::RenderBuffers(ComPtr<ID3D11DeviceContext> deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(StaticMeshVertex);
	offset = 0;

	// 렌더링할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정한다.
	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);

	// 렌더링할 수 있도록 인덱스 버퍼를 입력 어셈블러에서 활성으로 설정한다.
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 이 정점 버퍼에서 렌더링되어야 하는 기본 유형(이 경우 삼각형)을 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
