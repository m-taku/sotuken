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
	Shader					m_ps;							//!<�s�N�Z���V�F�[�_�[�B
	Shader					m_vs;							//!<���_�V�F�[�_�[�B
	Primitive				m_primitive;					//!<�v���~�e�B�u�B
	ID3D11ShaderResourceView*		m_textureSRV = nullptr;			//!<�e�N�X�`���B
	ID3D11SamplerState*		m_samplerState = nullptr;				//�T���v���[�X�e�[�g
	CMatrix					m_viewMatrix;									//�r���[�s��
	CMatrix					m_projMatrix;									//�v���W�F�N�V�����s��
};

