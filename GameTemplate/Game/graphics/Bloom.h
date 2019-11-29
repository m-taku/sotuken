#pragma once
class Bloom
{
public:
	Bloom();
	~Bloom();
	//������
	void Init(ID3D11ShaderResourceView* srv);
	//�`��
	void Draw();
	//�C�ɂ��Ȃ�
	void Release();
private:
	static const int DOWN_SAMPLE_NUM = 4;				//�_�E���T���v����
	RenderTarget m_luminanceRT;							//�P�x���o�p�����_�[�^�[�Q�b�g
	RenderTarget m_mixBlurTexRT;						//�{�P�摜�����p�����_�[�^�[�Q�b�g
	ID3D11ShaderResourceView* m_srcTexture = nullptr;	//���摜��SRV;
	GaussianBlur m_blur[DOWN_SAMPLE_NUM];				//�u���[�z��
	GaussianBlur m_finalBlur;							//�{�P�摜�̕i�����グ�邽�߂̃u���[
	ID3D11BlendState* m_disableBlendState = nullptr;	//���Z��������
	ID3D11BlendState* m_finalBlendState = nullptr;		//���Z�����L��
	PostEffect m_postEffect;	//�t���X�N���[���`��p
	Shader m_luminancePS;		//�P�x���o�p�s�N�Z���V�F�[�_�[
	Shader m_luminanceVS;		//�P�x���o�p���_�V�F�[�_�[
	Shader m_mixBlurPS;			//�{�P�摜�����p�s�N�Z���V�F�[�_�[
	Shader m_mixBlurVS;			//�{�P�摜�����p���_�V�F�[�_�[
	Shader m_finalBlendPS;		//���Z�����p�s�N�Z���V�F�[�_�[
	Shader m_finalBlendVS;		//���Z�����p���_�V�F�[�_�[
	int m_srcTexWidth = 0;		//���摜�̉���
	int m_srcTexHeight = 0;		//���摜�̏c��
};

