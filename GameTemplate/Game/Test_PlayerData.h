//武器属性
enum EnElement
{
	enFire,								//火属性
	enWater,							//水属性
	enThunder,							//雷属性
	enIce,								//氷属性
	enDragon,							//龍属性
	enPoison,							//毒属性
	enParalysis,						//麻痺属性
	enSleep,							//睡眠属性
	enNon_Elemental						//無属性
};
//耐性値
struct Vs 
{
	int VsFire = 0;						//火耐性
	int VsWater = 0;					//水属性
	int VsThunder = 0;					//雷属性
	int VsIce = 0;						//氷属性
	int VsDragon = 0;					//龍属性
};
struct PalyerData
{	
	char name[10] = "間抜け";			//名前
	int ranku = 1;						//ランク
	EnElement element = enNon_Elemental;	//武器属性
	int elementValue = 0;				//属性値
	Vs vs;								//耐性
	
    //クエスト中に変更される者たち
	int hp = 100;						//ＨＰ
	float stamina = 100;					//スタミナ
	int sharpness = NULL;				//切れ味
	int affinity = 0;					//会心率
	int attackPower = 1;				//攻撃力
	int defense = 1;					//防御力
};
struct PalyerExecuteParam
{
	int hp = 100;						//ＨＰ
	float stamina = 100;					//スタミナ
	int sharpness = NULL;				//切れ味
	int affinity = 0;					//会心率
	int attackPower = 1;				//攻撃力
	int defense = 1;					//防御力
};