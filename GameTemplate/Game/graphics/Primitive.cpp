#include "stdafx.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
	Release();
}

void Primitive::Release()
{
	m_vertexBuffer.Release();
	m_indexBuffer.Release();
}

bool Primitive::Create(
	D3D_PRIMITIVE_TOPOLOGY topology,
	int numVertex,
	int vertexStride,
	void* pSrcVertexBuffer,
	int numIndex,
	IndexBuffer::EnIndexType indexType,
	void* pSrcIndexBuffer)
{
	Release();
	bool result = m_vertexBuffer.Create(numVertex, vertexStride, pSrcVertexBuffer);
	if (!result)
	{
		throw;
		return false;
	}
	result = m_indexBuffer.Create(numIndex, indexType, pSrcIndexBuffer);
	if (!result) {
		throw;
		return false;
	}
	return true;
}

void Primitive::Draw(ID3D11DeviceContext& rc)
{
	UINT offset = 0;
	UINT stride = m_vertexBuffer.GetStride();
	rc.IASetVertexBuffers(0, 1, &m_vertexBuffer.GetBody(), &stride, &offset);
	IndexBuffer::EnIndexType type = m_indexBuffer.GetIndexType();
	rc.IASetIndexBuffer(m_indexBuffer.GetBody(),
		type == IndexBuffer::enIndexType_16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT,
		0
	);
	rc.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	rc.DrawIndexed(m_indexBuffer.GetNumIndex(), 0, 0);
}