#pragma once
class PostEffect
{
public:
	PostEffect();
	~PostEffect();

	void Init();

	void Draw();

	ID3D11ShaderResourceView* GetSRV()
	{
		return m_textureSRV;
	}

	void SetSRV(ID3D11ShaderResourceView* srv)
	{
		m_textureSRV = srv;
	}
private:
	Shader					m_ps;							//!<ピクセルシェーダー。
	Shader					m_vs;							//!<頂点シェーダー。
	Primitive				m_primitive;					//!<プリミティブ。
	ID3D11ShaderResourceView*		m_textureSRV = nullptr;			//!<テクスチャ。
	ID3D11SamplerState*		m_samplerState = nullptr;				//サンプラーステート
	CMatrix					m_viewMatrix;									//ビュー行列
	CMatrix					m_projMatrix;									//プロジェクション行列
};

