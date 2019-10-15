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
		stagi,
		time,
		doun,
		kane,
		monsu,
		iraisya,
		ikou,
		num
	};
	bool Start();
	void Draw();
protected:
	wchar_t name[num][256] = {L"NULL"};
	Text_Box* m_text[num] = { nullptr };
private:
	Sprite m_sprite[2];
	ShaderResourceView m_srv[2];

};
struct ma{
	struct taki
	{
		int BicBos_syurui;				//大型の種類			
		int kogata_pos[256];			//大型の敵のPOS
		int smorlBos_syurui;			//小型の種類
		int oogata_pos[256];			//小型の敵のPOS
	};
	struct settei
	{
		int jikan;				//制限時間
		int daunnsuu;			//残機数
		int hoysyuu;			//報酬金
	};
	struct staji
	{
		int tikeidata;			//マップデータ
		int doropuItem;			//ドロップアイテム（採取）
	};
};
