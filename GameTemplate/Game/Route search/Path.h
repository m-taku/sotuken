#pragma once
class Navimake;
/// <summary>
/// �p�X�f�[�^���g�����o�H�T���N���X�B
/// </summary>
class Path:public Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Path();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Path();
	/// <summary>
	/// �o�H�T��+�X���[�W���O�����B�i��肷�����ӁI�I�j
	/// </summary>
	/// <param name="sturt">
	/// �����|�W�V�����B�iCVector3�j
	/// </param>
	/// <param name="end">
	/// �@�ڕW�|�W�V�����B�iCVector3�j
	/// </param>
	void Course(CVector3 sturt, CVector3 end);
	/// <summary>
	/// �����o�ϐ����g���Ď��ɓ����ڕW�ʒu���擾�B(�����Ȃ�)
	/// </summary>
	/// <returns>
	/// �ڕW�ʒu�B�iCVector3�j
	/// </returns>
	const CVector3 PathPos();
	//�p�X�̃f�[�^�`��
	struct PasDate
	{
		PasDate() {
			//������
			LincNo[0] = -1;
			LincNo[1] = -1;
			LincNo[2] = -1;
			MoveCost = 0.0f;
			ParentDate = nullptr;
			to_DrstinCost=0.0f;
			No = -1;
		}
		~PasDate()
		{

		}
		int No;						//���g�̔ԍ�
		int LincNo[3];				//���g����̃����N��ԍ�
		float MoveCost;				//���g�̏ꏊ�܂ōs���R�X�g
		float to_DrstinCost;		//���g�̏ꏊ����ړI�n�܂ł̃R�X�g
		PasDate* ParentDate;		//�e�̃p�X�e�[�^�̃|�C���^
	};
	/// <summary>
	/// �p�X�f�[�^�ɂ��X���[�W���O�����B�icoursepasu��ύX�j
	/// </summary>
	/// <param name="pasu">
	/// �ړ��p�X�̏W�� �B(std::vector(int)*)
	/// </param>
	void Smoothing(std::vector<int>* pasu);
private:
	std::vector<int> m_coursepasu;							//�o�H�T���̌��ʁi�X���[�W���O�����ς݁j
	Navimake* m_pathdata = nullptr;							//�Z���i�|���S���j�̃f�[�^
	int m_nowNo = 0;										//�����Ԗڂ̃|�C���g�Ɍ������Ă��邩
};

