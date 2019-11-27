#pragma once
#ifdef  _DEBUG
/// <summary>
/// �f�o�b�N�p�̃x�N�g���\���N���X
/// </summary>
class VectorDraw:Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	/// <param name="position">
	/// �x�N�g���̎n�_�B(CVector3)
	/// </param>
	/// <param name="counnt">
	/// �x�N�g�������\�����邩�B�iint�j
	/// �i�C���X�^���V���O�̂��߁B�ォ��ύX�s�j
	/// </param>
	VectorDraw(CVector3 position,int counnt=1)
	{
		m_vector.Init(L"Assets/modelData/vectormodel.cmo");
		m_position = position;
		m_count = counnt;
	}
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~VectorDraw();
	/// <summary>
	///  1��1�ʂŃx�N�g����\��
	/// </summary>	
	/// <param name="posotion">
	/// �x�N�g���̎n�_(CVector3)
	/// </param>
	/// <param name="Vector">
	/// �����������x�N�g��(CVector3)
	/// </param>
	/// <param name="power">
	/// �x�N�g���̗́i�����j�ifloat�j
	/// </param>
	void Update(CVector3 posotion, CVector3 Vector, float power);
	/// <summary>
	/// 1��1�ʂœ_��\��
	/// </summary>
	void Update(CVector3 posotion);					
	/// <summary>
	/// �C���X�^���V���O�ɂ���ʂ̃x�N�g���\��
	/// </summary>
	/// <param name="posotion">
	/// �x�N�g���̎n�_�̔z��(std::vector(CVector3)::iterator)
	/// </param>
	/// <param name="Vector">
	/// �����������x�N�g���̔z��(std::vector(CVector3)::iterator&)
	/// </param>
	/// <param name="power">
	/// �x�N�g���̗́i�����j�̔z��istd::vector(float)::iterator�j
	/// </param>
	void Update(
			std::vector<CVector3>::iterator posotion,
			std::vector<CVector3>::iterator& Vector,
			std::vector<float>::iterator power);
	/// <summary>
	/// �C���X�^���V���O�ɂ���ʂ̓_��\��
	/// </summary>
	/// <param name="posotion">
	/// �\���������_�̔z��istd::vector(CVector3)&�j
	/// </param>
	void Update(const std::vector<CVector3>& posotion);
	/// <summary>
	/// �\���p�̊֐�
	/// �i����ɌĂ΂�Ȃ��̂Œ��ӁI�I�j
	/// </summary>
	void Draw();
private:
	SkinModel m_vector;							//�X�L�����f���̃C���X�^���X
	CVector3 vector = CVector3::Zero();			//�\���������x�N�g���̌����i�m�[�}���C�Y�j
	CVector3 m_position;		                //�\���������x�N�g���̌��_
	int m_count = 1;							//�\������x�N�g���i�_�j�̌�
	float Power = 0.0f;							//�\���������x�N�g���̑傫���ifloat�j
};
#else

class VectorDraw :Noncopyable
{
public:
	
	VectorDraw(CVector3 position, int counnt = 1){}
	~VectorDraw() {}
	void Update(CVector3 posotion, CVector3 Vector, float power) {}
	void Update(CVector3 posotion) {}
	void Update(
		std::vector<CVector3>::iterator posotion,
		std::vector<CVector3>::iterator& Vector,
		std::vector<float>::iterator power) {}
	void Update(const std::vector<CVector3>& posotion) {}
	void Draw() {}
};
#endif

