#pragma once
namespace smEngine {
	typedef unsigned char	GameObjectPrio;
	class IGameObject : public Noncopyable
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		using IGameObjectIsBase = IGameObject;
		IGameObject() :
			m_priority(0),
			m_isStart(false),
			m_isDead(false)
		{			
		}
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		virtual ~IGameObject()
		{

		}
		/*!
		*@brief	���s�D��x���擾�B
		*/
		GameObjectPrio GetPriority() const
		{
			return m_priority;
		}
		void StartWrapper()
		{
			if (!m_isStart&&m_isActive&&!m_isDead) {
				if (Start())
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
		void PostUpdateWrapper()
		{
			if (m_isStart&&m_isActive && !m_isDead)
			{
				PostUpdate();
			}
		}
		void UIDrawWrapper()
		{
			if (m_isStart&&m_isActive && !m_isDead)
			{
				UIDraw();
			}
		}
		void PostDrawWrapper()
		{
			if (m_isStart&&m_isActive && !m_isDead)
			{
				PostDraw();
			}
		}
		void DrawWrapper()
		{
			if (m_isStart&&m_isActive&&!m_isDead)
			{
				Draw();
			}
		}
		virtual bool Start() { return true; };
		virtual void Update() {};
		virtual void Draw() {};
		virtual void PostUpdate() {};
		virtual void UIDraw() {};
		virtual void PostDraw() {};
		virtual void OnDestroy() {};
		void SetActive(bool Active)
		{
			m_isActive = Active;
		};
		friend class IGameObjectManager;
	protected:
		GameObjectPrio	m_priority;						//���s�D��x�B
		bool m_isStart;						//Start�̊J�n�t���O�B
		bool m_isActive = true;				//Active�t���O�B
		bool m_isDead;						//���S�t���O�B
		bool m_isRegistDeadList;
		unsigned int m_nameKey = 0;			//���O�L�[�B
	};
}
