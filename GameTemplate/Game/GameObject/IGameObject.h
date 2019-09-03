#pragma once
namespace smEngine {
	typedef unsigned char	GameObjectPrio;
	class IGameObject : public Noncopyable
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		using IGameObjectIsBase = IGameObject;
		IGameObject() :
			m_priority(0),
			m_isStart(false),
			m_isDead(false)
		{			
		}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		virtual ~IGameObject()
		{

		}
		/*!
		*@brief	実行優先度を取得。
		*/
		GameObjectPrio GetPriority() const
		{
			return m_priority;
		}
		void StartWrapper()
		{
			if (!m_isStart&&m_isActive&&!m_isDead) {
				if (State())
				{
					m_isStart = true;
				}
			}
		}
		void UpdateWrapper()
		{
			if (m_isStart&&m_isActive&&!m_isDead)
			{
				Update();
			}
		}		
		void DrawWrapper()
		{
			if (m_isStart&&m_isActive&&!m_isDead)
			{
				Draw();
			}
		}
		virtual bool State() { return true; };
		virtual void Update() {};
		virtual void Draw() {};

		friend class IGameObjectManager;
	protected:
		GameObjectPrio	m_priority;						//実行優先度。
		bool m_isStart;						//Startの開始フラグ。
		bool m_isActive = true;				//Activeフラグ。
		bool m_isDead;						//死亡フラグ。
		bool m_isRegistDeadList;
		unsigned int m_nameKey = 0;			//名前キー。
	};
}
