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
		/*!
		*@brief	�J�����̑O����
		*/
		const CVector3& GetCameraFoward() const
		{
			return g_camera3D.GetForward();
		}
		/*!
		*@brief	�J�����̉E����
		*/
		const CVector3& GetCameraRight() const
		{
			return g_camera3D.GetRight();
		}
		/*!
		*@brief	�J�����̏����
		*/
		const CVector3& GetCameraUp() const
		{
			return g_camera3D.GetUp();
		}
		/*!
		*@brief	�J�����̃|�W�V����
		*/
		const CVector3& GetCameraPosition() const
		{
			return g_camera3D.GetPosition();
		}
		/*!
		*@brief	�J�����̃^�[�Q�b�g
		*/
		const CVector3& GetCameraTarget() const
		{
			return g_camera3D.GetTarget();
		}
		/*!
		*@brief �r���[�s��
		*/
		const CMatrix& GetCameraViewMatrix() const
		{
			return g_camera3D.GetViewMatrix();
		}
		/*!
		*@brief �v���W�F�N�V�����s��
		*/
		const CMatrix& GetCameraProjectionMatrix() const
		{
			return g_camera3D.GetProjectionMatrix();
		}

		/*!
		*@brief	�C���X�^���X(�V���O���g��)
		*/
		static GameCamera& Instance()
		{
			static GameCamera Instance;
			return Instance;
		}
	private:
		const float MAXDIST = 1000.0f;					//�J�����̍ő勗��
		CVector3 m_position = CVector3::Back()*MAXDIST;			//���_
		CVector3 m_target = CVector3::Zero();	//�����_
	};
	static GameCamera& smGameCamera()
	{
		return GameCamera::Instance();
	}
}


