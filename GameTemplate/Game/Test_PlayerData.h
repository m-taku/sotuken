//���푮��
enum EnElement
{
	enFire,								//�Α���
	enWater,							//������
	enThunder,							//������
	enIce,								//�X����
	enDragon,							//������
	enPoison,							//�ő���
	enParalysis,						//��ბ���
	enSleep,							//��������
	enNon_Elemental						//������
};
//�ϐ��l
struct Vs 
{
	int VsFire = 0;						//�Αϐ�
	int VsWater = 0;					//������
	int VsThunder = 0;					//������
	int VsIce = 0;						//�X����
	int VsDragon = 0;					//������
};
struct Palyer_Data_Test
{
	char name[10] = "�Ԕ���";
	int hp = 100;
	int stamina = 100;
	int ranku = 1;
	int attackPower = 1;				//�U����
	int defense = 1;					//�h���
	int sharpness = NULL;				//�؂ꖡ
	int affinity = 0;					//��S��
	EnElement element = enNon_Elemental;	//���푮��
	int elementValue = 0;				//�����l
	Vs vs;								//�ϐ�
};
