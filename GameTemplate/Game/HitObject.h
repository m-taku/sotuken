#pragma once
/// <summary>
/// �����蔻��p�̃I�u�W�F�N�g�f�[�^
/// </summary>
class HitObject :public Noncopyable
{
public:
	/// <summary>
	/// �I�u�W�F�N�g�̖��O
	/// �i�^�O�t�̖ړI�j
	/// </summary>
	enum objict_Name {
		enemy,
		player,
		name_num
	};
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HitObject();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~HitObject();
	/// <summary>
	/// �I�u�W�F�N�g�𐧍삷��
	/// </summary>
	/// <param name="pos">
	/// �|�W�V�����̃|�C���^
	/// �i�������Ώۂ̃|�C���^�j
	/// </param>
	/// <param name="radius">
	/// �I�u�W�F�N�g�̔��a
	/// </param>
	/// <param name="objict">
	/// �t�b�N�֐�
	/// </param>
	/// <param name="name">
	/// �I�u�W�F�N�g�̖��O
	/// </param>
	void Create(const CVector3* pos, float radius, std::function<void(float damage, CVector3 ObjDate)>  objict, objict_Name name);
	/// <summary>
	/// �I�u�W�F�N�g���m�̓����蔻��
	/// </summary>
	/// <param name="pos">
	/// ���Ă鑤�̃|�W�V����
	/// </param>
	/// <param name="Circle">
	/// ���Ă邪���̋��̂̔��a
	/// </param>
	/// <param name="damage">
	/// ���������ꍇ�̗^����ϐ�
	/// �i����̓_���[�W���j
	/// </param>
	/// <returns>
	/// �������true
	/// </returns>
	bool HitTest(CVector3 originpos,CVector3 pos, float Circle, float damage);
	/// <summary>
	/// �I�u�W�F�N�g�̖��O�i�^�O�j�̃Q�b�g
	/// </summary>
	/// <returns>
	/// �I�u�W�F�N�g�̖��O�A�^�O�iobjict_Name �j
	/// </returns>
	objict_Name Getobjict_Name()
	{
		return m_name;
	}
	const CVector3 Getpos()
	{
		if (m_pos != nullptr) {
			return *m_pos;
		}
		else
		{
			return CVector3::Zero();
		}
	}
private:
	const CVector3* m_pos = nullptr;						//�I�u�W�F�N�g�̃|�W�V����
	float m_radius = 0.0f;									//�I�u�W�F�N�g�̔��a
	objict_Name m_name = name_num;							//�I�u�W�F�N�g�̖��O�i�^�O�j
	std::function<void(float damage, CVector3 ObjDate)>  m_fuk;				//���������ꍇ�ɌĂт����֐�
};