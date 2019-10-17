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
	Shader					m_ps;							//!<�s�N�Z���V�F�[�_�[�B
	Shader					m_vs;							//!<���_�V�F�[�_�[�B
	Primitive				m_primitive;					//!<�v���~�e�B�u�B
	ID3D11SamplerState*		m_samplerState = nullptr;				//�T���v���[�X�e�[�g
	CMatrix					m_viewMatrix;									//�r���[�s��
	CMatrix					m_projMatrix;									//�v���W�F�N�V�����s��
};

