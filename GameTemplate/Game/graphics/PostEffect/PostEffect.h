#pragma once
class PostEffect
{
public:
	PostEffect();
	~PostEffect();

	void Init();

	void Draw();

	void SetPS(Shader* PS)
	{
		m_ps = PS;
	}
	void SetVS(Shader* VS)
	{
		m_vs = VS;
	}
	void Release();
private:
	Shader*					m_ps;							//!<ピクセルシェーダー。
	Shader*					m_vs;							//!<頂点シェーダー。
	Primitive				m_primitive;					//!<プリミティブ。
	ID3D11SamplerState*		m_samplerState = nullptr;				//サンプラーステート
	CMatrix					m_viewMatrix;									//ビュー行列
	CMatrix					m_projMatrix;									//プロジェクション行列
};

