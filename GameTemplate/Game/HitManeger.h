#pragma once
#include "HitObject.h"
/// <summary>
/// �ȈՂȓ����蔻��
/// </summary>
namespace smEngine {
	class HitManager
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		HitManager()
		{

		}
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~HitManager()
		{
			Release();
		}
		/// <summary>
		/// Hit�I�u�W�F�N�g�̃C���X�^���X�̃Q�b�g
		/// </summary>
		/// <returns>
		/// Hit�I�u�W�F�N�g�̃C���X�^���X��
		/// </returns>
		static inline HitManager& GetHitObjict()
		{
			static HitManager t;
			return t;
		}
		const HitObject* Create(const CVector3* pos, float radius, std::function<void(float damage, CVector3 ObjDate)> objict, HitObject::objict_Name name)
		{
			auto Object = new HitObject;
			Object->Create(pos, radius, objict, name);
			m_objict.push_back(Object);
			return Object;
		}
		/// <summary>
		/// �����蔻��̔����i���j
		/// </summary>
		/// <param name="pos">
		/// �����蔻��̒��S���W
		/// </param>
		/// <param name="Circle">
		/// �����蔻��̋��̑傫��
		/// </param>
		/// <param name="damage">
		/// �^�������_���[�W�̗�
		/// </param>
		/// <param name="name">
		/// ���Ă����Ώ�
		/// </param>
		/// <returns>
		/// ����������true
		/// </returns>
		bool HitTest(CVector3 originpos, CVector3 pos, float Circle, float damage, HitObject::objict_Name name)
		{
			bool furag = false;
			for (auto hit : m_objict)
			{
				//���Ă������̂���������
				if (hit->Getobjict_Name() == name) {
					//�����蔻����s���B
					if (hit->HitTest(originpos,pos, Circle, damage)) {
						furag = true;
					}
				}
			}
			return furag;
		}
		void Deleteobjict(const HitObject* objict)
		{
			m_objict.erase(std::find(m_objict.begin(), m_objict.end(), objict));
			if (objict != nullptr) {
				delete objict;
			}
		}
		void Release()
		{
			for (auto k : m_objict)
			{
				delete k;
			}
			m_objict.clear();
		}
	private:
		std::list<HitObject*> m_objict;
	};
	static HitManager& GetHitObjict()
	{
		return  HitManager::GetHitObjict();
	}
}