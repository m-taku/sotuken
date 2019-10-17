#pragma once
class PostEffect
{
public:
	PostEffect();
	~PostEffect();

	void Init();

	void Draw();

	void SetPS(const char* filepath, const char* funcName)
	{
		m_ps.Load(filepath, funcName, Shader::EnType::PS);
	}
	void SetVS(const char* filepath, const char* funcName)
	{
		m_vs.Load(filepath, funcName, Shader::EnType::VS);
	}
	void Release();
private:
	Shader					m_ps;							//!<ピクセルシェーダー。
	Shader					m_vs;							//!<頂点シェーダー。
	Primitive				m_primitive;					//!<プリミティブ。
	ID3D11SamplerState*		m_samplerState = nullptr;				//サンプラーステート
	CMatrix					m_viewMatrix;									//ビュー行列
	CMatrix					m_projMatrix;									//プロジェクション行列
};

