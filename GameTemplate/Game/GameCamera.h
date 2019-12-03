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
			g_camera3D.SetTarget(target);
		}
		/*!
		*@brief	���_���Z�b�g
		*/
		void SetPosition(CVector3& position) {

			g_camera3D.SetPosition(position);
		}
		/*!
		*@brief	���_���Z�b�g
		*/
		void SetUp(CVector3& up) {

			g_camera3D.SetUp(up);
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
		*@brief �r���[�s��
		*/
		const CMatrix& GetPreCameraViewMatrix() const
		{
			return m_piricamera.mView;
		}
		/*!
		*@brief �v���W�F�N�V�����s��
		*/
		const CMatrix& GetCameraProjectionMatrix() const
		{
			return g_camera3D.GetProjectionMatrix();
		}
		/*!
		*@brief �v���W�F�N�V�����s��
		*/
		const CMatrix& GetPreCameraProjectionMatrix() const
		{
			return m_piricamera.mProj;
		}
		void Updatepre()
		{
			m_piricamera.mProj = g_camera3D.GetProjectionMatrix();
			m_piricamera.mView = g_camera3D.GetViewMatrix();
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
		struct puriCamara {
			CMatrix mView = CMatrix::Identity();
			CMatrix mProj = CMatrix::Identity();
		};
		const float MAXDIST = 1000.0f;					//�J�����̍ő勗��
		CVector3 m_position = CVector3::Back()*MAXDIST;			//���_
		CVector3 m_target = CVector3::Zero();	//�����_
		puriCamara m_piricamera;
	};
	static GameCamera& smGameCamera()
	{
		return GameCamera::Instance();
	}
}


