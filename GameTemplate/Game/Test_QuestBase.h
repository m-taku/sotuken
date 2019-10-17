#pragma once
class Text_Box;
class Test_QuestBase : public IGameObject
{
public:
	Test_QuestBase();
	~Test_QuestBase();
	enum Quest
	{
		titor,
		moster,
		kane,
		time,
		doun,
		iraisya,
		bikou,
		num
	};
	enum Moster
	{
		monnsu,
		monnsu2,
		monnsu3,
		mosternum
	};
	bool Start();
	void Draw();
protected:
	char  name[num][256] = { NULL };
	Text_Box* m_text[num] = { nullptr };
	Moster MonsterNo = mosternum;
	wchar_t EnglishMosterName[num][256] = {L"monnsu",L"monnsu2", L"monnsu3"};
	char JapanMosterName[num][256] = { "�����X�^�[1", "�����X�^�[2","�����X�^�[3" };
private:
	Sprite m_sprite[2];
	ShaderResourceView m_srv[2];
	CVector2 m_Textpos[num] = {
		{0.0f,400.0f},
	{ 0.0f,-120.0f },
	{ -180.0f,-200.0f },
	{ 180.0f,-200.0f },
	{ 0.0f,-280.0f },
	{ -200.0f,-380.0f },
	{ 0.0f,-420.0f }
	};

};
struct ma{
	struct taki
	{
		int BicBos_syurui;				//��^�̎��			
		int kogata_pos[256];			//��^�̓G��POS
		int smorlBos_syurui;			//���^�̎��
		int oogata_pos[256];			//���^�̓G��POS
	};
	struct settei
	{
		int jikan;				//��������
		int daunnsuu;			//�c�@��
		int hoysyuu;			//��V��
	};
	struct staji
	{
		int tikeidata;			//�}�b�v�f�[�^
		int doropuItem;			//�h���b�v�A�C�e���i�̎�j
	};
};
