#include "stdafx.h"
#include "Navimake.h"
#include "Physics/CollisionAttr.h"
#include "title.h"
#include <fstream>


Navimake::Navimake()
{
	wchar_t moveFilePath[256];
	swprintf_s(moveFilePath, L"Assets/modelData/Ground1.cmo");
	m_model.Init(moveFilePath);
	//���b�V���R���C�_�[���쐬�B
	m_meshCollider.CreateFromSkinModel(m_model, nullptr);
	////���b�V���R���C�_�[���璸�_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̏���Get����
	//std::fstream fout;
	//fout.open(failpas, std::ios::binary | std::ios::in);
	//fout.read((char*)data, sizeof(PalyerData));
	//fout.close();
	swprintf_s(moveFilePath, L"Assets/Navimake/nabe.nav");
	FILE* fp = _wfopen(moveFilePath, L"rb");
	if (fp == nullptr) {
		int No = 0;
		for (int i = 0; i < m_meshCollider.Getok(); i++) {
			auto vertex = m_meshCollider.Getvertex(i);
			auto index = m_meshCollider.GetIndex(i);
			for (int i = 0; i < index.size();) {
				//�P�̃|���S���i�Z���j�ɕ�����
				SData* data = new SData;
				data->position[0] = vertex[index[i++]];
				data->position[1] = vertex[index[i++]];
				data->position[2] = vertex[index[i++]];
				auto vector1 = data->position[1] - data->position[0];
				auto vector2 = data->position[2] - data->position[0];
				CVector3 jiku;
				vector1.Normalize();
				vector2.Normalize();
				jiku.Cross(vector2, vector1);
				jiku.Normalize();
				float j = acos(jiku.Dot(CVector3::AxisY()));
				if (CMath::DegToRad(45.0f) <= j)
				{
					delete data;
					continue;
				}
				//�f�[�^�̏�����
				data->linkNoList[0] = -1;
				data->linkNoList[1] = -1;
				data->linkNoList[2] = -1;
				data->cost[0] = -1;
				data->cost[1] = -1;
				data->cost[2] = -1;
				data->No = No++;
				//�o�����Z�����璆�S�����߂�B
				data->centerposition = Searchcenter(data->position);
				//�ł����|���S���i�Z���j����push_back����
				m_seru.push_back(data);
				//��������ContactTest�ɂ��p�X���O����
				{
					bool frag = false;
					CVector3 Circle;
					Circle = data->position[0] - data->centerposition;
					//�ł����|���S��������~�̑傫�����v�Z
					for (int i = 1; i < 2; i++) {
						if ((data->position[i] - data->centerposition).Length() >= Circle.Length())
						{
							Circle = data->position[i] - data->centerposition;
						}
					}
					//�|���S��������R���W��������
					CreateRigidBody(80.0f, 5000.0f, data->centerposition);
					g_physics.ContactTest(m_rigidBody, [&](const btCollisionObject& contactObject)
					{
						frag = true;
					});
					if (frag)
					{
						m_seru.erase(
							std::remove(m_seru.begin(), m_seru.end(), data),
							m_seru.end());
						delete data;
						No--;
					}
					g_physics.RemoveRigidBody(m_rigidBody);
					m_collider.Release();
				}
			}
		}
		//�������烊���N���̐���

		for (auto Major = m_seru.begin(); Major != m_seru.end(); Major++)
		{
			//�܂��匳�ɂȂ�P�̃|���S���i�Z���j�����肷��
			SData& MajorData = *(*Major);
			for (auto Former = m_seru.begin(); Former != m_seru.end(); Former++)
			{
				const SData& ComparisonData = **Former;
				//�匳�ȊO�̃|���S���i�Z���j����������
				if (MajorData.No != ComparisonData.No)
				{
					int Commonvertex = 0;
					int vertexNo[2] = { 0 };
					for (int i = 0; i < 3 && Commonvertex < 2; i++)
					{
						//�匳�̃|���S���i�Z���j�P�̒��_�ɑ΂���
						CVector3 Majorposition = MajorData.position[i];
						for (int j = 0; j < 3; j++)
						{
							//CVector3 Comparisonposition = ComparisonData.position[j];
							CVector3 distance;
							distance = ComparisonData.position[j] - Majorposition;
							//�����̂��������|���S���i�Z���j���ׂĂ̒��_�Ƌ����𑪂�
							if (distance.Length() <= 0.1f)
							{
								//�v�Z��������������Ȃ��߂��ꍇ��
								//�匳�̒��_�ԍ���ۑ�����
								vertexNo[Commonvertex] = i;
								Commonvertex++;
								//�匳�̒��_�ԍ��̕ۑ����Q�i�����j�ɂȂ�����
								if (Commonvertex == 2)
								{
									//�����N���Ƃ��ĕۑ�����
									if ((vertexNo[0] == 0 && vertexNo[1] == 1)
										|| (vertexNo[1] == 0 && vertexNo[0] == 1)
										) {
										//���_�ԍ�0-1�ɗאڂ��Ă����
										MajorData.linkNoList[0] = ComparisonData.No;
										MajorData.cost[0] = (MajorData.centerposition - ComparisonData.centerposition).Length();
									}
									else if ((vertexNo[0] == 1 && vertexNo[1] == 2)
										|| (vertexNo[1] == 1 && vertexNo[0] == 2)
										) {
										//���_�ԍ�1-2�ɗאڂ��Ă����
										MajorData.linkNoList[1] = ComparisonData.No;
										MajorData.cost[1] = (MajorData.centerposition - ComparisonData.centerposition).Length();
									}
									else if ((vertexNo[0] == 0 && vertexNo[1] == 2)
										|| (vertexNo[1] == 0 && vertexNo[0] == 2)
										) {
										//���_�ԍ�0-2�ɗאڂ��Ă����
										MajorData.linkNoList[2] = ComparisonData.No;
										MajorData.cost[2] = (MajorData.centerposition - ComparisonData.centerposition).Length();
									}
									break;
								}
							}
						}
					}
				}
			}
		}

		FILE* fp1 = _wfopen(L"Assets/Navimake/nabe.nav", L"wb");
		int num = 0;
		num = m_seru.size();
		fwrite(&num, sizeof(int), 1, fp1);
		for (int i = 0; i < m_seru.size(); i++) {
			fwrite(m_seru[i], sizeof(SData),1, fp1);
		}
		fclose(fp1);
		//std::fstream fout;
		//fout.open(moveFilePath, std::ios::binary | std::ios::out);
		//int num = m_seru.size();
		//fout.write((char*)num, sizeof(int));
		//for (int i = 0; i < num; i++) {
		//	auto na = (SData*)m_seru[i];
		//	fout.write((char*)na, sizeof(SData));

		//}
		//fout.close();
		//fp
	}
	else {
		int num = 0;
		fread(&num, sizeof(num), 1, fp);

		//���g�R�s�[���邽�߂̃��������h�J�b�Ɗm�ہB
		for (int i = 0; i < num; i++) {
			SData* keyframes = new SData;
			fread(keyframes, sizeof(SData), 1, fp);
			m_seru.push_back(keyframes);
		}
		fclose(fp);
	}
	
	////��������f�o�b�N�p�̒��_�\��
	//{
	//	std::vector<CVector3> centerposition;		//���_�̓����z��
	//	m_vector.push_back(new VectorDraw(m_seru[0]->centerposition,m_seru.size()));
	//	for (int i = 0; i < m_seru.size(); i++)
	//	{
	//		centerposition.push_back(m_seru[i]->centerposition);
	//	}
	//	m_vector[m_vector.size() - 1]->Update(centerposition);
	//}

	////��������f�o�b�N�p�̃����N�\��
	//{
	//	std::vector<CVector3> centerposition_rinc;		//���_�̔z��
	//	std::vector<CVector3> Vectorlist_rinc;			//���_�ƒ��_���Ȃ��x�N�g���̔z��
	//	std::vector<float> Vectorpore_rinc;				//�x�N�g���̒���
	//	for (int i = 0; i < m_seru.size(); i++)
	//	{
	//		CVector3 c_position;
	//		c_position = m_seru[i]->centerposition;
	//		for (int j = 0; j < 3; j++) {
	//			if (m_seru[i]->linkNoList[j] != -1)
	//			{
	//				CVector3 Vector = CVector3::Zero();
	//				Vector = m_seru[m_seru[i]->linkNoList[j]]->centerposition - c_position;
	//				centerposition_rinc.push_back(c_position);
	//				Vectorlist_rinc.push_back(Vector);
	//				Vectorpore_rinc.push_back(Vector.Length() / 3.0f);
	//			}
	//		}
	//	}
	//	m_vector.push_back(new VectorDraw(m_seru[0]->centerposition, centerposition_rinc.size()));
	//	m_vector[m_vector.size()-1]->Update(centerposition_rinc.begin(), Vectorlist_rinc.begin(), Vectorpore_rinc.begin());
	//}
	//���̂��쐬�A
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider; //���̂Ɍ`��(�R���C�_�[)��ݒ肷��B
	rbInfo.mass = 0.0f;
	rbInfo.pos = { 0.0f,0.0f,0.0f };
	rbInfo.rot = CQuaternion::Identity();
	m_rigidBody.Create(rbInfo);
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Object);
	//���̂𕨗����[���h�ɒǉ�����B
	g_physics.AddRigidBody(m_rigidBody);
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}


Navimake::~Navimake()
{
	//���̂𕨗����[���h����������B
	g_physics.RemoveRigidBody(m_rigidBody);
	//for (int i = 0; i < m_vector.size(); i++)
	//{
	//	//�f�o�b�N�p�̃x�N�g�����g���Ă����Delete����
	//	delete m_vector[i];
	//}
	for (int i = 0; i < m_seru.size(); i++)
	{
		//�p�X�f�[�^�����ׂč폜����
		delete m_seru[i];
	}
	m_seru.clear();
}
void Navimake::Draw()
{
	//���f����Draw
	m_model.Draw(enNormal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	//for (int i = 0; i < m_vector.size(); i++) {
	//	//�f�o�b�N�p�x�N�g��������Ε\��
	//	m_vector[i]->Draw();
	//}
}
CVector3 Navimake::Searchcenter(const CVector3 (&pos)[3])
{
	//���_�v�Z���s��
	CVector3 centerpos=CVector3::Zero();
	centerpos.x = (pos[0].x + pos[1].x + pos[2].x) / 3;
	centerpos.y = (pos[0].y + pos[1].y + pos[2].y) / 3;
	centerpos.z = (pos[0].z + pos[1].z + pos[2].z) / 3;
	return centerpos;
}
const std::vector<Path::PasDate*> Navimake::FindLinc(Path::PasDate& date, int endNo,float cost)const
{
	std::vector<Path::PasDate*> dete;
	dete.resize(3);
	for (int i = 0; i < 3; i++) {
		Path::PasDate* pasDate = new Path::PasDate;
		if (m_seru[date.No]->linkNoList[i] != -1) 
		{
			//�����N��񂪂���΃f�[�^�̂�ǉ�����
			pasDate->ParentDate = &date;
			pasDate->No = m_seru[date.No]->linkNoList[i];
			pasDate->LincNo[0]= m_seru[pasDate->No]->linkNoList[0];
			pasDate->LincNo[1] = m_seru[pasDate->No]->linkNoList[1];
			pasDate->LincNo[2] = m_seru[pasDate->No]->linkNoList[2];
			pasDate->MoveCost = m_seru[date.No]->cost[i]+ cost;
			pasDate->to_DrstinCost= (m_seru[endNo]->centerposition - m_seru[pasDate->No]->centerposition).Length();
		}
		else
		{
			//�����N��񂪂Ȃ���ΐe�̂�
			pasDate->ParentDate = &date;
		}
		//�f�[�^��ς�
		dete[i] = pasDate;
	}
	//�ł����f�[�^��Ԃ�
	return dete;
}
void Navimake::DebugVector(const std::vector<int>& posudate)
{
	//�o�H�f�[�^�i�p�X�ԍ��f�[�^�j���f�o�b�N�p�̃f�[�^�쐬
	std::vector<CVector3> Vectorlist;			//�x�N�g���̔z��
	std::vector<CVector3> centerposition;		//���_�̔z��
	std::vector<float> Vectorpore;				//�x�N�g���̒���
	for (int i = 0; i < posudate.size() - 1; i++)
	{
		CVector3 c_position;
		c_position = m_seru[posudate[i]]->centerposition;
		CVector3 Vector = CVector3::Zero();
		Vector = m_seru[posudate[i + 1]]->centerposition - c_position;
		centerposition.push_back(c_position);
		Vectorlist.push_back(Vector);
		Vectorpore.push_back(Vector.Length() / 3.0f);
	}
	//�ł����f�[�^��VectorDraw�ɑ}��
	//if (centerposition.size() >= 1) {
	//	m_vector.push_back(new VectorDraw(m_seru[0]->centerposition, centerposition.size()));
	//	if (centerposition.size() != 1) {
	//		m_vector[m_vector.size() - 1]->Update(centerposition.begin(), Vectorlist.begin(), Vectorpore.begin());
	//	}
	//	else
	//	{
	//		m_vector[m_vector.size() - 1]->Update(centerposition[0], Vectorlist[0], Vectorpore[0]);
	//	}
	//}
}
//�X���[�W���O�p�̃R�[���o�b�N�N���X
struct Collision : public btCollisionWorld::ConvexResultCallback
{
	bool NextNo = false; //���̔ԍ�
	btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
														//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{

		if (convexResult.m_hitCollisionObject == me) {
			//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
			return 0.0f;
		}
		//�L�����N�^�[�͏�Q���ł͂Ȃ�
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character) {

			return 0.0f;
		}
		NextNo = true;
		return 0.0f;
	}
};
bool Navimake::CollisionTest(int sturtNo, int nextNo)
{
	//�X���[�Y����ۂ̉��̓����蔻��i�Œ�l�Ȃ񂾂�Ȃ��A�A�A�A�A�j
	CapsuleCollider m_collide;						//�R���C�_�[�B
	m_collide.Create(hight, ballsize);
	CVector3 nextPosition = m_seru[nextNo]->centerposition;
	//���̓����蔻��̒��S���W + ����*0.1�̍��W��posTmp�ɋ��߂�B
	CVector3 posTmp = m_seru[sturtNo]->centerposition;
	posTmp.y += hight + ballsize + hight * 0.1f;
	//���C���쐬�B
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
	start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
	//�I�_�͎��̈ړ���BXZ���ʂł̏Փ˂𒲂ׂ�̂ŁAy��posTmp.y��ݒ肷��B
	end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

	Collision callback;

	callback.me = m_rigidBody.GetBody();
	//�Փˌ��o�B
	g_physics.ConvexSweepTest((const btConvexShape*)m_collide.GetBody(), start, end, callback);

	//�Փ˂������ǂ���
	return callback.NextNo;
}
void Navimake::CreateRigidBody(float radius, float height, const CVector3& position) {
	auto m_position = position;
	//�R���W�����쐬�B
	auto m_radius = radius;
	auto m_height = height;
	m_collider.Create(radius, height);
	//���̂��������B
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Create(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//���̂̈ʒu���X�V�B
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	g_physics.AddRigidBody(m_rigidBody);
}	