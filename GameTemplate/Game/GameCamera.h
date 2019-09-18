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
			m_target = target;
		}
		/*!
		*@brief	視点をセット
		*/
		void SetPosition(CVector3& position) {
			m_position = position;
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
		*@brief	インスタンス(シングルトン)
		*/
		static GameCamera& Instance()
		{
			static GameCamera Instance;
			return Instance;
		}
	private:
		const float MAXDIST = 1000.0f;					//カメラの最大距離
		CVector3 m_position = CVector3::Zero();			//視点
		CVector3 m_target = CVector3::Back()*MAXDIST;	//注視点
	};
	static GameCamera& smGameCamera()
	{
		return GameCamera::Instance();
	}
}


