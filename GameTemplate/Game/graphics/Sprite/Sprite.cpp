#include "stdafx.h"
#include "Sprite.h"

namespace smEngine {
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	const CVector2	Sprite::DEFAULT_PIVOT = { 0.0f, 0.0f };
	Sprite::Sprite()
	{
		m_viewMatrix.MakeLookAt(
			{ 0.0f,0.0f,-5.0f },
			CVector3::Zero(),
			CVector3::Up()
		);
		m_projMatrix.MakeOrthoProjectionMatrix(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			1.0f,
			10.0f
		);
	}
	Sprite::~Sprite()
	{
		if (m_samplerState != nullptr)
		{
			m_samplerState->Release();
		}
	}
	void Sprite::Init(ShaderResourceView& tex, float w, float h)
	{
		m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
		m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
		m_size.x = w / FRAME_BUFFER_W;
		m_size.y = h / FRAME_BUFFER_H;
		float halfW = w * 0.5f;
		float halfH = h * 0.5f;

		SSimpleVertex vertices[] =
		{
				{
					CVector4(-halfW,-halfH,0.0f,1.0f),
					CVector2(0.0f,1.0f),
				},
				{
					CVector4(halfW,-halfH,0.0f,1.0f),
					CVector2(1.0f,1.0f),
				},
				{
					CVector4(-halfW, halfH, 0.0f, 1.0f),
					CVector2(0.0f, 0.0f)
				},
				{
					CVector4(halfW, halfH, 0.0f, 1.0f),
					CVector2(1.0f, 0.0f)
				}
		};


		short indices[] = { 0,1,2,3 };

		m_primitive.Create(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			4,
			sizeof(SSimpleVertex),
			vertices,
			4,
			IndexBuffer::enIndexType_16,
			indices
		);

		m_textureSRV = &tex;
		m_cb.Create(nullptr, sizeof(SSpriteCB));

		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
		m_isInited = true;
		Update(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());

	}
	/*!
	*@brief	更新
	*@param[in]	trans		平行移動。
	*@param[in]	rot			回転。
	*@param[in]	scale		拡大。
	*/
	void Sprite::Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot)
	{
		if (m_isInited == false) {
			//初期化されていない。
			return;
		}
		//ピボットを考慮に入れた平行移動行列を作成。
		//ピボットは真ん中が0.0, 0.0、左上が-1.0f, -1.0、右下が1.0、1.0になるようにする。
		CVector2 localPivot = pivot;
		localPivot.x *= FRAME_BUFFER_W / 2.0f;
		localPivot.y *= FRAME_BUFFER_H / 2.0f;
		localPivot.x *= -2.0f;
		localPivot.y *= -2.0f;
		//画像のハーフサイズを求める。
		CVector2 halfSize = m_size;
		halfSize.x *= 0.5f;
		halfSize.y *= 0.5f;
		CMatrix mPivotTrans;

		mPivotTrans.MakeTranslation(
			{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
		);
		CMatrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(trans);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world.Mul(mPivotTrans, mScale);
		m_world.Mul(m_world, mRot);
		m_world.Mul(m_world, mTrans);

	}
	void Sprite::Draw(ID3D11DeviceContext* DeviceContext)
	{
		if (m_isInited == false) {
			//初期化されていない。
			return;
		}
		if (m_textureSRV == nullptr) {
			abort();
			return;
		}
		SSpriteCB cb;
		cb.WVP = m_world;
		cb.WVP.Mul(cb.WVP, m_viewMatrix);
		cb.WVP.Mul(cb.WVP, m_projMatrix);
		cb.mulColor = m_mulColor;

		DeviceContext->UpdateSubresource(m_cb.GetBody(), 0, NULL, &cb, 0, 0);
		DeviceContext->VSSetConstantBuffers(0, 1, &m_cb.GetBody());
		DeviceContext->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		DeviceContext->VSSetShaderResources(0, 1, &m_textureSRV->GetBody());
		DeviceContext->PSSetShaderResources(0, 1, &m_textureSRV->GetBody());
		DeviceContext->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), NULL, 0);
		DeviceContext->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), NULL, 0);
		DeviceContext->IASetInputLayout(m_vs.GetInputLayout());
		DeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		m_primitive.Draw(*DeviceContext);
	}
}