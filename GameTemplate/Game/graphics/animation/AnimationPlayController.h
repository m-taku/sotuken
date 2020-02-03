/*!
 * @brief	アニメーション再生コントローラ。
 */

#pragma once

class Skeleton;
class AnimationClip;
class Bone;
class Animation;

/*!
* @brief	アニメーションの再生コントローラ。
* @details
*  一つのアニメーションクリップに対してアニメーションを進めて、ローカルポーズを計算します。
*/
class AnimationPlayController{
public:
	/*!
	* @brief	コンストラクタ。
	*/
	AnimationPlayController()
	{
	}
	/*!
	* @brief	デストラクタ。
	*/
	~AnimationPlayController()
	{
	}
	/*!
	*@brief	初期化。
	*/
	void Init(Skeleton* skeleton);
	/*!
	* @brief	アニメーションクリップの変更。
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
	*@brief	補完率を取得。
	*/
	float GetInterpolateRate() const
	{
		if (m_interpolateEndTime <= 0.0f) {
			return 1.0f;
		}
		return min( 1.0f, m_interpolateTime / m_interpolateEndTime );
	}
	/*!
	* @brief	アニメーションを進める。
	*@param[in]	deltaTime		アニメーションを進める時間。
	*/
	void Update(float deltaTime, Animation* animation);
	/*!
	* @brief	ローカルボーン行列を取得。
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
	* @brief	再生中？
	*/
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
/// イベント区間内かどうか
/// </summary>
/// <returns>
/// trueで区間内
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
	/// イベントの名前を取得します
	/// </summary>
	/// <returns>
	/// イベント名（NULLだとイベント外）
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
	*@brief	ループ再生開始する時の処理。
	*/
	void StartLoop();
private:
	AnimationClip*			m_animationClip = nullptr;		//!<アニメーションクリップ。
	int						m_currentKeyFrameNo = 0;		//!<現在再生中のキーフレーム番号。
	float					m_time = 0.0f;
	std::vector<CMatrix>	m_boneMatrix;				//!<このコントローラで再生中のアニメーションのボーン行列。
	float					m_interpolateTime;			//!<補完時間
	float					m_interpolateEndTime;		//!<補完終了時間
	bool					m_isPlaying = false;		//!<再生中？
	std::vector<bool>		m_Event;					//!<アニメーションイベントがどっち？
	std::vector<const wchar_t*>	m_eventname;			//!<今のイベントの名前
};