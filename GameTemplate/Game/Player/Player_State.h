#pragma once
/// <summary>
/// �v���C���[�X�e�[�g�̊��N���X
/// </summary>
class Player;
class Player_State :public Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="player">
	///�v���C���[�̃|�C���^�B�iPlayer*�j
	/// </param>
	Player_State(Player* player);
	/// <summary>
	/// �f�X�g���N�^�i�j
	/// </summary>
	virtual ~Player_State();
	/// <summary>
	/// �A�b�v�f�[�g�B
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �A�b�v�f�[�g�B
	/// </summary>
	virtual void Draw()
	{

	}
protected:
	Player* m_player = nullptr;				//�v���C���[�̃C���X�^���X
};

