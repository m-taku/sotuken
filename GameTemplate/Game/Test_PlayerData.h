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
struct PalyerData
{	
	char name[10] = "�Ԕ���";			//���O
	int ranku = 1;						//�����N
	EnElement element = enNon_Elemental;	//���푮��
	int elementValue = 0;				//�����l
	Vs vs;								//�ϐ�
	
    //�N�G�X�g���ɕύX�����҂���
	int hp = 100;						//�g�o
	float stamina = 100;					//�X�^�~�i
	int sharpness = NULL;				//�؂ꖡ
	int affinity = 0;					//��S��
	int attackPower = 1;				//�U����
	int defense = 1;					//�h���
};
struct PalyerExecuteParam
{
	int hp = 100;						//�g�o
	float stamina = 100;					//�X�^�~�i
	int sharpness = NULL;				//�؂ꖡ
	int affinity = 0;					//��S��
	int attackPower = 1;				//�U����
	int defense = 1;					//�h���
};