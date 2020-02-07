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
	void SetMulColor(float color) {
		m_mulColor = { color ,color ,color ,1.0f };
	}
	void Release();
private:
	CVector4 m_mulColor = CVector4::White();
	ConstantBuffer m_cb;
	Shader*					m_ps;							//!<�s�N�Z���V�F�[�_�[�B
	Shader*					m_vs;							//!<���_�V�F�[�_�[�B
	Primitive				m_primitive;					//!<�v���~�e�B�u�B
	ID3D11SamplerState*		m_samplerState = nullptr;				//�T���v���[�X�e�[�g
	CMatrix					m_viewMatrix;									//�r���[�s��
	CMatrix					m_projMatrix;									//�v���W�F�N�V�����s��
};

