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
		monsu,
		kane,
		time,
		doun,
		iraisya,
		bikou,
		num
	};
	bool Start();
	void Draw();
protected:
	char  name[num][256] = { NULL };
	Text_Box* m_text[num] = { nullptr };
	wchar_t monsuta[3][256] = { L"�����X�^�[1", L"�����X�^�[2",L"�����X�^�[3" };
private:
	Sprite m_sprite[2];
	ShaderResourceView m_srv[2];

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
