#include "stdafx.h"
#include "PhysicsDebugDraw.h"


void PhysicsDebugDraw::Init()
{
	{
		//バッファ作成
		m_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CVector4) * static_cast<int>(m_vertexBuffer.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &m_vertexBuffer[0];
		HRESULT hr = g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_VertexBuffer);
		if (FAILED(hr)) {
			throw;
		}
	}
	m_vs.Load("Assets/shader/linePrimitive.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/linePrimitive.fx", "PSMain", Shader::EnType::PS);
	//ConstantBuffer用のバッファ定義を作成する。
	{
		//ワールド行列等のためのバッファ
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = (((sizeof(SConstantBuffer) - 1) / 16) + 1) * 16;	//16バイトアライメントに切りあげる。
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		HRESULT hr = g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
		if (FAILED(hr)) {
			throw;
		}
	}
	{
		//色用のバッファ
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = (((sizeof(CVector4) - 1) / 16) + 1) * 16;	//16バイトアライメントに切りあげる。
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		HRESULT hr = g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb2);
		if (FAILED(hr)) {
			throw;
		}
	}
	//m_cb.Create(nullptr, sizeof(SConstantBuffer));
	//m_cb2.Create(nullptr, sizeof(CVector4));
}
void PhysicsDebugDraw::BeginDraw()
{
	DeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	m_numLine = 0;
}
void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//物理エンジンからもらった情報を
	int baseIndex = m_numLine * 2;
	m_vertexBuffer[baseIndex].Set(CVector3(from.x(), from.y(), from.z()));
	m_vertexBuffer[baseIndex + 1].Set(CVector3(to.x(), to.y(), to.z()));
	m_numLine++;
}
void PhysicsDebugDraw::EndDraw()
{
	//定数バッファの更新。
	SConstantBuffer cb;
	cb.mView = g_camera3D.GetViewMatrix();
	cb.mProj = g_camera3D.GetProjectionMatrix();	
	DeviceContext->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), NULL, 0);
	DeviceContext->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), NULL, 0);
	{
		//色決定
		CVector4 colr;
		colr = { 0.0f,0.0f,0.0f,1.0f };
		DeviceContext->UpdateSubresource(m_cb2, 0, NULL, &colr, 0, 0);
		DeviceContext->PSSetConstantBuffers(1, 1, &(m_cb2));
	}
	{

		DeviceContext->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
		DeviceContext->VSSetConstantBuffers(0, 1, &(m_cb));
	}

	//入力レイアウトを設定。
	DeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	DeviceContext->UpdateSubresource(m_VertexBuffer, 0, NULL, &m_vertexBuffer[0], 0, 0);
	//m_primitive.Draw(*DeviceContext, m_numLine * 2);
	
	//表示処理
	
	UINT ofset = 0;
	UINT stride = sizeof(CVector4);
	(*DeviceContext).IASetVertexBuffers(0, 1, &(m_VertexBuffer), &stride, &ofset);
	//プリミティブのトポロジーを設定。
	(*DeviceContext).IASetPrimitiveTopology(m_topology);
	//描画。
	(*DeviceContext).Draw(m_numLine * 2, 0);
}