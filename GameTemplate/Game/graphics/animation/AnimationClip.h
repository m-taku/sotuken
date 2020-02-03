/*!
 *@brief	アニメーションクリップ。
 */

#pragma once
//////////////////////////////////////////////////////////////////////
// 各種構造体
//////////////////////////////////////////////////////////////////////
/*!
*@brief	アニメーションクリップのヘッダー。
*/
struct AnimClipHeader {
	std::uint32_t numKey;				//!<キーフレームの数。
	std::uint32_t numAnimationEvent;	//!<アニメーションイベントの数。
};
/*!
*@brief	アニメーションイベント
*@アニメーションイベントは未対応。
* やりたかったら自分で実装するように。
*/
struct AnimationEventData {
	float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
	std::uint32_t eventNameLength;		//!<イベント名の長さ。
};
/*!
*@brief	キーフレーム。
*/
struct Keyframe {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CMatrix transform;			//!<トランスフォーム。
};
/*!
*@brief	キーフレーム。
*/
struct KeyframeRow {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CVector3 transform[4];		//!<トランスフォーム。
};
struct EventDate {
	float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
	std::wstring eventNameLength;		//!<イベント名
};
/// <summary>
/// アニメーションイベントクラス
/// </summary>
/// <remarks>
/// キーとキーの間を取るためのデータです
/// </remarks>
class AnimationEvent : Noncopyable
{
public:
	AnimationEvent()
	{

	}
	~AnimationEvent()
	{
		for (auto date : m_date)
		{
			delete date;
		}
		m_date.clear();
	}
	/// <summary>
	/// イベントのデータのセット
	/// </summary>
	/// <param name="date">
	/// イベントデータ（EventDate*）
	/// </param>
	void SetEventDete(EventDate* date)
	{
		m_date.push_back(date);
	}
	/// <summary>
	/// 発生しているイベントの名前の取得
	/// </summary>
	/// <returns>
	/// 発生しているイベントの名前（wchar_t*）
	/// </returns>
	const wchar_t* GetEventname(int No) const
	{
		return m_date[No]->eventNameLength.c_str();
	}
	/// <summary>
	/// イベントの発生させる時間のゲット
	/// </summary>
	/// <returns>
	/// イベントの発生時間（秒）
	/// </returns>
	const float GetinvokeTime(int No)
	{
		return m_date[No]->invokeTime;
	}
	/// <summary>
	/// イベントの総数
	/// </summary>
	/// <returns>
	/// イベントデータの数
	/// </returns>
	const int GetDateSize()
	{
		return m_date.size();
	}
private:
	std::vector<EventDate*> m_date;
};
/*!
*@brief	アニメーションクリップ。
*/
//#include"Animation.h"
class Animation;
class AnimationClip  {
public:
	//タイプ量が長ったらしくて、うざいのでstd::vector<KeyFrame*>の別名定義。
	using keyFramePtrList = std::vector<Keyframe*>;
	/*!
	* @brief	コンストラクタ
	*/
	AnimationClip() 
	{
	}
	/*!
	*@brief	デストラクタ。
	*/
	~AnimationClip();
	/*!
	*@brief	アニメーションクリップをロード。
	*@param[in]	filePath	ファイルパス。
	*/
	void Load(const wchar_t* filePath);

	/*!
	*@brief	ループする？
	*/
	bool IsLoop() const
	{
		return m_isLoop;
	}
	/*!
	*@brief	ループフラグを設定する。
	*/
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	void SetAnimation(Animation* animation)
	{
		m_Animation = animation;
	}
	/*!
	*@brief
	*/
	const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
	{
		return m_keyFramePtrListArray;
	}
	const keyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFramList;
	}
	const std::vector<AnimationEvent*>& GetAnimationEventlist()const
	{
		return m_AnimationEventlist;
	}
private:
	
	bool m_isLoop = false;									//!<ループフラグ。
	std::vector<Keyframe*> m_keyframes;						//全てのキーフレーム。
	std::vector<keyFramePtrList> m_keyFramePtrListArray;	//ボーンごとのキーフレームのリストを管理するための配列。
															//例えば、m_keyFramePtrListArray[0]は0番目のボーンのキーフレームのリスト、
															//m_keyFramePtrListArray[1]は1番目のボーンのキーフレームのリストといった感じ。
	std::vector<AnimationEvent*> m_AnimationEventlist;		//アニメーションイベントの配列
	Animation* m_Animation = nullptr;
	keyFramePtrList* m_topBoneKeyFramList = nullptr;
};
