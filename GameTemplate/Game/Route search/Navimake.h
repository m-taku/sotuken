#pragma once
#include"physics/MeshCollider.h"
#include "physics/RigidBody.h"
#include"character/CharacterController.h"
#include"Path.h"
/// <summary>
/// �o�H�T���p�̃��b�V���f�[�^�B
/// </summary>
class Navimake :public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Navimake();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Navimake();
	/// <summary>
	/// Gameobject����p������Draw�֐�
	/// </summary>
	void Draw() override;
	/// <summary>
	/// �|�W�V�������g�����o�X�ԍ������B
	/// </summary>
	/// <param name="position">
	/// �|�W�V�����B(CVector3)
	/// </param>
	/// <returns>
	/// �p�X�ԍ��B�iint�j
	/// </returns>
	int FindPos_No(CVector3 position) const
	{
		CVector3 closepos;
		closepos.x = FLT_MAX;
		closepos.y = FLT_MAX;
		closepos.z = FLT_MAX;
		int No = 0;
		for (int i = 0; i < m_seru.size(); i++)
		{
			CVector3 pos = position - m_seru[i]->centerposition;
			if (closepos.Length() >= pos.Length())
			{
				//��ԋ߂��p�X�̔ԍ�������
				No = m_seru[i]->No;
				closepos = pos;
			}
		}
		return No;
	}
	/// <summary>
	/// �ԍ����킩���Ă���ꍇ�̒��S���W�����B
	/// </summary>
	/// <param name="No">
	/// �p�X�ԍ��B�iint�j
	/// </param>
	/// <returns>
	/// �|�W�V�����B�iCVector3�j
	/// </returns>
	CVector3 FindNo_Pos(int No) const
	{
		return m_seru[No]->centerposition;
	}
	/// <summary>
	/// �p�X�̓������z����g�����f�o�b�N�\���p�֐�
	/// </summary>
	/// <param name="posudate">
	///  �p�X�ԍ��̓������ϒ��z��(std::vector(int))
	/// </param>
	void DebugVector(const std::vector<int>& posudate);
	/// <summary>
	/// �e��PasDate�ƃR�X�g�ƖړI�n���g���ă����N����������
	/// </summary>
	/// <param name="date">
	/// �e��PasDate
	/// </param>
	/// <param name="endNo">
	/// �ړI�n�iint�j
	/// </param>
	/// <param name="cost">
	/// �e�̃p�X�܂ł̃R�X�g�ifloat�j
	/// </param>
	/// <returns>
	/// �����N���
	/// </returns>
	const std::vector<Path::PasDate*> FindLinc(Path::PasDate& date, int endNo, float cost) const;
	/// <summary>
	/// �X���[�Y�����̎��̒n�`�Ƃ̓����蔻��
	/// </summary>
	/// <param name="sturtNo">
	/// ���C���΂��n�_
	/// </param>
	/// <param name="nextNo">
	/// ���C���΂��I�_
	/// </param>
	/// <returns>
	/// �Փ˂�true�A���Փ˂�false
	/// </returns>
	bool CollisionTest(int sturtNo, int nextNo);
	void CreateRigidBody(float radius, float height, const CVector3& position);
private:
	CVector3 Searchcenter(const CVector3(&pos)[3]);	//���_�����߂�֐�
	/// <summary>
	/// Cell�\����
	/// </summary>
	/// <remarks>
	/// �i�r�Q�[�V�������b�V���̍ŏ��P�ʂƂ�Cell�\���́B
	/// �Z�����\������R�p�`�̒��_�ƁA�א�Cell���Ȃǂ������Ă���B
	/// </remarks>
	struct SData {
		//CVector3				normal;				//�@��
		CVector3				position[3];		//�O�p�`1�̍��W
		CVector3				centerposition;		//���S���W
		int	                    linkNoList[3];		//�����N�̃��X�g
		int                     No;					//�����̔ԍ�
		float                   cost[3];			//�����N��ɍs���ۂ̃R�X�g
	};
	MeshCollider m_meshCollider;					//���b�V�����
	CapsuleCollider		m_collider;					//�R���C�_�[�B
	SkinModel m_model;								//���f���f�[�^
	std::vector<SData*> m_seru;						//�O�p�`�P�̃f�[�^
	RigidBody m_rigidBody;			               	//���́B
	static const int hight = 80;					//CollisionTest�p�̃J�v�Z���̕�
	static const int ballsize = 500;				//CollisionTest�p�̃J�v�Z���̍���
};