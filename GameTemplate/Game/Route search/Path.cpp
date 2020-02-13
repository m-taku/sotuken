#include "stdafx.h"
#include "Path.h"
#include"Navimake.h"

Path::Path()
{
}


Path::~Path()
{

}
bool Path::Course(CVector3 sturt, CVector3 end)
{
	m_stop = false;
	m_pathdata = FindGO<Navimake>("Navimake");
	if (m_pathdata == nullptr)
	{
		return false;
	}
	m_nowNo = 0;
	m_coursepasu.clear();
	//��������|�W�V�������g���ăp�X�ԍ����擾
	int startNo = m_pathdata->FindPos_No(sturt);
	int endNo = m_pathdata->FindPos_No(end);
	//�X�^�[�g�̃p�X�ԍ����g���čŏ��̃f�[�^�����
	PasDate* Date = new PasDate;
	Date->No = startNo;
	//�I�[�v���m�[�h�ƃN���[�Y�m�[�h�𐧍�
	std::vector<PasDate*> open;
	std::vector<PasDate*> close;
	//�I�[�v���ɃX�^�[�g�̃f�[�^��ς�
	open.push_back(Date);
	auto NextPas = open[0];
	float CurrentCost = 0.0f;
	//�f�[�^�̔ԍ����Ō�̔ԍ��ɂȂ�܂ő�����
	while (NextPas->No != endNo)
	{
		if (m_stop)
		{
			for (auto ereas : open) {
				delete ereas;
			}
			for (auto ereas : close) {
				delete ereas;
			}
			return false;
		}
		//�����N�f�[�^��T��
		auto LincPas = m_pathdata->FindLinc(*NextPas, endNo, CurrentCost);
		//�T���I������̂ŃN���[�Y����B
		open.erase(
			std::remove(open.begin(), open.end(), NextPas),
			open.end());
		close.push_back(NextPas);
		//�����N�̐�����
		for (int j = 0; j < 3; j++)
		{
			//�����������N�f�[�^�������
			if (-1 != LincPas[j]->No)
			{
				//�I�[�v���ɐς�
				open.push_back(LincPas[j]);
				for (int i = 0; i < open.size(); i++) {
					if (open[i] == LincPas[j])
					{
						break;
					}
					//�I�[�v���̒��ɓ����ԍ��̃p�X�ԍ��������
					if (open[i]->No == LincPas[j]->No) {
						//�f�[�^���r���邵�ăR�X�g�̒Ⴂ�ق����c��
						if (open[i]->to_DrstinCost <= LincPas[j]->to_DrstinCost)
						{
							delete LincPas[j];
							open.erase(
								std::remove(open.begin(), open.end(), LincPas[j]),
								open.end());
							break;
						}
						else
						{
							delete open[i];
							open.erase(
								std::remove(open.begin(), open.end(), open[i]),
								open.end());
							break;

						}
					}
				}
				//�N���[�Y�ɂ������p�X�ԍ������邩��������
				for (int i = 0; i < close.size(); i++) {
					if (close[i]->No == LincPas[j]->No) {
						if (close[i]->to_DrstinCost <= LincPas[j]->to_DrstinCost) {
							delete  LincPas[j];
							open.erase(
								std::remove(open.begin(), open.end(), LincPas[j]),
								open.end());
							break;
						}
						else {
							delete close[i];
							close.erase(
								std::remove(close.begin(), close.end(), close[i]),
								open.end());
							break;
						}
					}
				}
				//�f�[�^�����̏I���
			}
			//�����N�f�[�^���Ȃ��ꍇ�f�[�^����������
			else
			{
				delete LincPas[j];
			}
		}
		//�I�[�v�����[���͂��������̂ŋ����I������
		if (open.size() <= 0)
		{
			break;
		}
		float costnV2 = FLT_MAX;
		//�I�[�v���̒�����ŏ��̃R�X�g���������邵
		//���������f�[�^�����̃I�[�v������f�[�^�ɂ���
		for (auto i : open)
		{
			if ((i->MoveCost + i->to_DrstinCost) <= costnV2) {
				costnV2 = i->MoveCost + i->to_DrstinCost;
				NextPas = i;
			}
		}
		//���̃����N�܂ł̍ŏ��R�X�g���ړ��R�X�g�Ƃ���
		CurrentCost = NextPas->MoveCost;
	}
	if (open.size() <= 0)
	{
		//�I�[�v�����Ȃ��͕̂s��Ȃ̂ŏI������
		m_coursepasu.push_back(startNo);
	}
	else
	{
		//�G���h����X�^�[�g�Ɍ������ă����N��i�߂Ă���
		while (NextPas->No != startNo) {
			m_coursepasu.push_back(NextPas->No);
			NextPas = NextPas->ParentDate;
		}
		m_coursepasu.push_back(NextPas->No);
		//�����܂܂��Ƌt�̔ԍ��Ȃ̂�
		//���o�[�X���ăX�^�[�g����ɂ���
		std::reverse(m_coursepasu.begin(), m_coursepasu.end());
		if (m_coursepasu.size() > 2)
		{
			//�����ŃX���[�W���O����
			Smoothing(&m_coursepasu);
		}
		//�f�o�b�N��������΂���
		//m_pathdata->DebugVector(m_coursepasu);
	}
	//���ׂďI������̂Ńf�[�^����������
	for (auto ereas : open) {
		delete ereas;
	}
	for (auto ereas : close) {
		delete ereas;
	}

	return true;
}
void Path::Smoothing(std::vector<int>* pasu)
{
	//�X���[�Y�X�^�[�g���̔ԍ�
	std::vector<int> pa;
	int start = (*pasu)[0];
	pa.push_back(start);
	int Next;
	int wait = 0;
	//1��΂���2�Ԗڂ��猟�����J�n�ɂ���B
	for (int i = 1; i < pasu->size(); i++) 
	{
		Next = (*pasu)[i];
		if (m_pathdata->CollisionTest(start, Next))
		{
			if (wait == 0) {
				start = (*pasu)[i - 1];
				wait++;
				pa.push_back(start);
			}
		}
		else
		{
			if (wait != 0) {
				pa.push_back(Next);
				wait = 0;
			}
		}
	}
	pa.push_back(Next);
	*pasu = pa;
}
const CVector3 Path::PathPos()
{
	if (m_coursepasu.size()-1 > m_nowNo) {
		return m_pathdata->FindNo_Pos(m_coursepasu[m_nowNo++]);
	}
	else
	{
		return m_pathdata->FindNo_Pos(m_coursepasu[m_nowNo]);
	}
}