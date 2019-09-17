#pragma once

namespace smEngine {
	class GameCamera
	{
	public:
		GameCamera();
		~GameCamera();
		void Init();		//������
		void Update();		//�J�����̍X�V
		/*!
		*@brief	���������̂��Z�b�g
		*/
		void SetTarget(CVector3& target) {
			m_target = target;
		}
		/*!
		*@brief	���_���Z�b�g
		*/
		void SetPosition(CVector3& position) {
			m_position = position;
		}

		static GameCamera& Instance()
		{
			static GameCamera Instance;
			return Instance;
		}
	private:
		const float MAXDIST = 1000.0f;					//�J�����̍ő勗��
		CVector3 m_position = CVector3::Zero();			//���_
		CVector3 m_target = CVector3::Back()*MAXDIST;	//�����_
	};
	static GameCamera& smGameCamera()
	{
		return GameCamera::Instance();
	}
}


