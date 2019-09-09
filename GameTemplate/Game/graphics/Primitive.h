#pragma once

class Primitive : Noncopyable
{
public:
	Primitive();
	~Primitive();
	bool Create(
		D3D_PRIMITIVE_TOPOLOGY topology,
		int numVertex,
		int vertexStride,
		void* pSrcVertexBuffer,
		int numIndex,
		IndexBuffer::EnIndexType indexType,
		void* pSrcIndexBuffer
	);
	void Release();
	void Draw(ID3D11DeviceContext& rc);
private:
	VertexBuffer			m_vertexBuffer;		//!<頂点バッファ。
	IndexBuffer			m_indexBuffer;		//!<インデックスバッファ。
	D3D_PRIMITIVE_TOPOLOGY	m_topology;			//!<トポロジー。
};

