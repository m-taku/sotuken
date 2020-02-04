#include "stdafx.h"
#include "Test_QuestBase.h"
#include"Text_Box/Text_Box.h"
#include "Stage/QuestStage.h"
#include"Player.h"
#include"Enemy/Enemy.h"
#include"Stage/Town.h"
#include"QuestManager.h"
#include"QuestResult.h"
#include"../Enemyname/MonsterList.h"

Test_QuestBase::Test_QuestBase()
{

}


Test_QuestBase::~Test_QuestBase()
{
	for (int i = 0; i < num; i++)
	{
		DeleteGO(m_text[i]);
	}
}
bool Test_QuestBase::Start()
{
	//�N�G�X�g�̎��̃f�[�^
	m_srv[0].CreateFromDDSTextureFromFile(L"Assets/sprite/QuestForm.dds");
	m_sprite[0].Init(m_srv[0], 800.0f, 1000.0f);
	m_sprite[0].Update({ 0.0f,0.0f,0.0f }, CQuaternion::Identity(), CVector3::One()); 
	//�����̕\��
	for (int i = 0; i < Quest::num; i++) {
		m_text[i] = NewGO<Text_Box>(10, "Tessfagn");
		m_text[i]->Init(name[i],m_Textpos[i]);
		m_text[i]->SetSpeed(0.0f);
	}
	wchar_t FileName[256];
	swprintf_s(FileName,L"Assets/sprite/Monster/%s.dds", EnglishMosterName[MonsterNo]);
	m_srv[1].CreateFromDDSTextureFromFile(FileName);
	m_sprite[1].Init(m_srv[1], 350.0f, 350.0f);
	m_sprite[1].Update({ 0.0f,150.0f,0.0f }, CQuaternion::Identity(), CVector3::One());
	m_sprite[1].SetMulColor({ 1.0f,1.0f,2.0f,1.0f });
	m_isStart = true;
	m_time *= 60.0f;
	return true;
}
void Test_QuestBase::CreateQuest()
{
	//�_�E�����ݒ�
	m_doun = m_Maxdoun;
	m_Quest = true;
	m_nowtime = 0.0f;
	for (auto na : m_text)
	{
		if (na != nullptr) {
			na->SetActive(false);
		}
	}
	monster* monnsu;
	switch (MonsterNo)
	{
	case Test_QuestBase::monnsu:
		monnsu =new dorakomesu({ 0.0f,0.0f,0.0f });
		break;
	case Test_QuestBase::monnsu2:
		monnsu = new dorakomesu({ 0.0f,0.0f,0.0f });
		break;
	case Test_QuestBase::monnsu3:
		monnsu = new dorakomesu({ 0.0f,0.0f,0.0f });
		break;
	default:
		break;
	}
	m_target = monnsu->GetEnemy();
	//�G�l�~�[������

	//�X�e�[�W�ɒʒm�����2�ȏ�ł����


	//�N�G�X�g�Ɋ֌W���錸�Z������΂����ł���(���Ƃ�)�B
}
void Test_QuestBase::Update()
{
	if (m_Quest) {
		//�N�G�X�g���s�֌W
		{
			//�N�G�X�g���Ԋ֌W
			m_nowtime += GetFrameDeltaTime();
			if (m_time <= m_nowtime)
			{
				ChengResult(false);
			}
			else {
				//�_�E���֌W
				if (m_nowdoun) {
					debugtime++;
					if (debugtime >= 120) {
						m_doun--;
						if (m_doun <= 0) {
							ChengResult(false);
						}
						else
						{
							auto pla = FindGO<Player>("player");
							pla->TransitionState(Player::StateQuestMove);
						}
						debugtime = 0;
						m_nowdoun = false;
					}
				}
			}
		}
		//�N�G�X�g��������
		{
			if (m_target ==nullptr)
			{

				m_target = FindGO<Enemy>("enemy");
			}
			else {
				//�|���ꂽ��
				if (m_target->GetState() == Enemy::StateDead) {
					ChengResult(true);
				}
			}
		}
	}
}
void Test_QuestBase::ChengResult(bool flag)
{
	m_Quest = false;
	SetActive(m_Quest);
	NewGO<QuestResult>(0,"Result")->SetClear(flag);
}
void Test_QuestBase::PostDraw()
{
	if (!m_Quest) {
		for (int i = 0; i < 2; i++) {
			m_sprite[i].Draw(g_graphicsEngine->GetD3DDeviceContext());
		}
	}
}