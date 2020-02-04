//•Ší‘®«
enum EnElement
{
	enFire,								//‰Î‘®«
	enWater,							//…‘®«
	enThunder,							//—‹‘®«
	enIce,								//•X‘®«
	enDragon,							//—´‘®«
	enPoison,							//“Å‘®«
	enParalysis,						//–ƒáƒ‘®«
	enSleep,							//‡–°‘®«
	enNon_Elemental						//–³‘®«
};
//‘Ï«’l
struct Vs 
{
	int VsFire = 0;						//‰Î‘Ï«
	int VsWater = 0;					//…‘®«
	int VsThunder = 0;					//—‹‘®«
	int VsIce = 0;						//•X‘®«
	int VsDragon = 0;					//—´‘®«
};
struct Palyer_Data_Test
{
	char name[10] = "ŠÔ”²‚¯";
	int hp = 100;
	int stamina = 100;
	int ranku = 1;
	int attackPower = 1;				//UŒ‚—Í
	int defense = 1;					//–hŒä—Í
	int sharpness = NULL;				//Ø‚ê–¡
	int affinity = 0;					//‰ïS—¦
	EnElement element = enNon_Elemental;	//•Ší‘®«
	int elementValue = 0;				//‘®«’l
	Vs vs;								//‘Ï«
};
