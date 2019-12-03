#pragma once

namespace smEngine {
	class GameCamera
	{
	public:
		GameCamera();
		~GameCamera();
		void Init();		//初期化
		void Update();		//カメラの更新
		/*!
		*@brief	見たいものをセット
		*/
		void SetTarget(CVector3& target) {
			g_camera3D.SetTarget(target);
		}
		/*!
		*@brief	視点をセット
		*/
		void SetPosition(CVector3& position) {

			g_camera3D.SetPosition(position);
		}
		/*!
		*@brief	視点をセット
		*/
		void SetUp(CVector3& up) {

			g_camera3D.SetUp(up);
		}
		/*!
		*@brief	カメラの前方向
		*/
		const CVector3& GetCameraFoward() const
		{
			return g_camera3D.GetForward();
		}
		/*!
		*@brief	カメラの右方向
		*/
		const CVector3& GetCameraRight() const
		{
			return g_camera3D.GetRight();
		}
		/*!
		*@brief	カメラの上方向
		*/
		const CVector3& GetCameraUp() const
		{
			return g_camera3D.GetUp();
		}
		/*!
		*@brief	カメラのポジション
		*/
		const CVector3& GetCameraPosition() const
		{
			return g_camera3D.GetPosition();
		}
		/*!
		*@brief	カメラのターゲット
		*/
		const CVector3& GetCameraTarget() const
		{
			return g_camera3D.GetTarget();
		}
		/*!
		*@brief ビュー行列
		*/
		const CMatrix& GetCameraViewMatrix() const
		{
			return g_camera3D.GetViewMatrix();
		}
		/*!
		*@brief ビュー行列
		*/
		const CMatrix& GetPreCameraViewMatrix() const
		{
			return m_piricamera.mView;
		}
		/*!
		*@brief プロジェクション行列
		*/
		const CMatrix& GetCameraProjectionMatrix() const
		{
			return g_camera3D.GetProjectionMatrix();
		}
		/*!
		*@brief プロジェクション行列
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
		*@brief	インスタンス(シングルトン)
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
		const float MAXDIST = 1000.0f;					//カメラの最大距離
		CVector3 m_position = CVector3::Back()*MAXDIST;			//視点
		CVector3 m_target = CVector3::Zero();	//注視点
		puriCamara m_piricamera;
	};
	static GameCamera& smGameCamera()
	{
		return GameCamera::Instance();
	}
}


