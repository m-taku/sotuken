#pragma once
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
		ikou
	};
	bool Start();
	void Update();
	Sprite m_sprite[2];
	ShaderResourceView m_srv[2];
	wchar_t name[256][256] = {L"NULL"};

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
