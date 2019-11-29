#pragma once
class GaussianBlur
{
public:
	GaussianBlur();
	~GaussianBlur();
	void Init(ID3D11ShaderResourceView* srv, float blurIntensity = 25.0f);
	void InitDownSample(ID3D11ShaderResourceView* srv, int downSampleRate = 2, float blurIntensity = 25.0f);
	void InitOriginalResolusion(ID3D11ShaderResourceView* srv, float blurIntensity = 25.0f);
	void InitUpScaling(ID3D11ShaderResourceView* srv, int upScaleRate = 2, float blurIntensity = 25.0f);

	void Execute(float distance = 2.0f, float division = 1.0f);

	ID3D11ShaderResourceView* GetBlurSRV()
	{
		return m_passYRT.GetShaderResourceView();
	}

	void Release();
private:
	void UpdateWeight();				//�d�݂̌v�Z
	static const int WEIGHT_NUM = 8;
	//�p�����[�^�[�̍\����
	struct SBlurPalam {

		float weights[WEIGHT_NUM];	//�d��
		float distance = 2.0f;		//�s�N�Z���Ԃ̋���
		float division = 1.0f;		//�������p(distance���ϓ�����)
	};
	SBlurPalam m_blurPalam;			//�p�����[�^�[�̃C���X�^���X
	ID3D11ShaderResourceView* m_SRV = nullptr;		//���摜��SRV
	RenderTarget m_passXRT;		//X�u���[�p�̃����_�[�^�[�Q�b�g
	RenderTarget m_passYRT;		//Y�u���[�p�̃����_�[�^�[�Q�b�g
	PostEffect m_postEffect;	//�`�����
	Shader m_XVS;				//X�u���[�p�̒��_�V�F�[�_�[
	Shader m_YVS;				//Y�u���[�p�̒��_�V�F�[�_�[
	Shader m_PS;				//�s�N�Z���V�F�[�_�[
	int m_srvWidth = 0.0f;		//���摜�̉���
	int m_srvHeight = 0.0f;		//���摜�̏c��
	float m_blurIntensity = 25.0f;	//�u���[���x
	ConstantBuffer m_cb;			//�p�����[�^�[�p�̒萔�o�b�t�@�[
};

