/*!
 * @brief	�A�j���[�V�����Đ��R���g���[���B
 */

#pragma once

class Skeleton;
class AnimationClip;
class Bone;
class Animation;

/*!
* @brief	�A�j���[�V�����̍Đ��R���g���[���B
* @details
*  ��̃A�j���[�V�����N���b�v�ɑ΂��ăA�j���[�V������i�߂āA���[�J���|�[�Y���v�Z���܂��B
*/
class AnimationPlayController{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	AnimationPlayController()
	{
	}
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~AnimationPlayController()
	{
	}
	/*!
	*@brief	�������B
	*/
	void Init(Skeleton* skeleton);
	/*!
	* @brief	�A�j���[�V�����N���b�v�̕ύX�B
	*/
	void ChangeAnimationClip(AnimationClip* clip)
	{
		m_animationClip = clip;
		m_currentKeyFrameNo = 0;
		m_Event.resize(m_animationClip->GetAnimationEventlist().size());
		m_eventname.resize(m_animationClip->GetAnimationEventlist().size());
		m_time = 0.0f;
		m_isPlaying = true;
	}
	void SetInterpolateTime(float interpolateTime)
	{
		if (interpolateTime < 0.0f) {
			interpolateTime = 0.0f;
		}
		m_interpolateEndTime = interpolateTime;
		m_interpolateTime = 0.0f;
	}
	/*!
	*@brief	�⊮�����擾�B
	*/
	float GetInterpolateRate() const
	{
		if (m_interpolateEndTime <= 0.0f) {
			return 1.0f;
		}
		return min( 1.0f, m_interpolateTime / m_interpolateEndTime );
	}
	/*!
	* @brief	�A�j���[�V������i�߂�B
	*@param[in]	deltaTime		�A�j���[�V������i�߂鎞�ԁB
	*/
	void Update(float deltaTime, Animation* animation);
	/*!
	* @brief	���[�J���{�[���s����擾�B
	*/
	const std::vector<CMatrix>& GetBoneLocalMatrix() const
	{
		return m_boneMatrix;
	}
	AnimationClip* GetAnimClip() const
	{
		return m_animationClip;
	}
	/*!
	* @brief	�Đ����H
	*/
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
/// �C�x���g��ԓ����ǂ���
/// </summary>
/// <returns>
/// true�ŋ�ԓ�
/// </returns>
	bool IsEvent(int No = 0)const
	{
		if (No <= (int)(m_Event.size()))
		{
			return m_Event[No - 1];
		}
		else {
			return false;
		}
	}
	/// <summary>
	/// �C�x���g�̖��O���擾���܂�
	/// </summary>
	/// <returns>
	/// �C�x���g���iNULL���ƃC�x���g�O�j
	/// </returns>
	const wchar_t* GetEventName(int No = 0)
	{
		if (No <= m_eventname.size() - 1)
		{
			return m_eventname[No];
		}
		else {
			return L"NULL";
		}
	}
private:
	
	/*!
	*@brief	���[�v�Đ��J�n���鎞�̏����B
	*/
	void StartLoop();
private:
	AnimationClip*			m_animationClip = nullptr;		//!<�A�j���[�V�����N���b�v�B
	int						m_currentKeyFrameNo = 0;		//!<���ݍĐ����̃L�[�t���[���ԍ��B
	float					m_time = 0.0f;
	std::vector<CMatrix>	m_boneMatrix;				//!<���̃R���g���[���ōĐ����̃A�j���[�V�����̃{�[���s��B
	float					m_interpolateTime;			//!<�⊮����
	float					m_interpolateEndTime;		//!<�⊮�I������
	bool					m_isPlaying = false;		//!<�Đ����H
	std::vector<bool>		m_Event;					//!<�A�j���[�V�����C�x���g���ǂ����H
	std::vector<const wchar_t*>	m_eventname;			//!<���̃C�x���g�̖��O
};