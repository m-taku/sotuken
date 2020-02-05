#pragma once
/// <summary>
/// �v���C���[�X�e�[�g�̊��N���X
/// </summary>
class Player;
enum State {
	StateTownMove,			//�ړ���
	StateQuestMove,
	StateAvoid,
	StateAttack,
	StateAttackMode,
	StateWate,
	Statedeath
};
class PlayerState :public Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="player">
	///�v���C���[�̃|�C���^�B�iPlayer*�j
	/// </param>
	PlayerState(Player* player);
	/// <summary>
	/// �f�X�g���N�^�i�j
	/// </summary>
	virtual ~PlayerState();
	/// <summary>
	/// �A�b�v�f�[�g�B
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �h���[
	/// </summary>
	virtual void Draw()
	{

	}
	virtual void DamageAction(float damage) {

	}
	int DownHp(int Damage);
protected:
	Player* m_player = nullptr;				//�v���C���[�̃C���X�^���X
};

