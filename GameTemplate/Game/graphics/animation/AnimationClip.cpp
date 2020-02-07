/*!
 *@brief	アニメーションクリップ。
 */

#include "stdafx.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"


AnimationClip::~AnimationClip()
{
	//キーフレームを破棄。
	for (auto& keyFrame : m_keyframes) {
		delete keyFrame;
	}
	for (auto Event : m_AnimationEventlist)
	{
		delete Event;
	}
	m_Animation->deleteAnimation(this);
}

void AnimationClip::Load(const wchar_t* filePath)
{
	FILE* fp = _wfopen(filePath, L"rb");
	if (fp == nullptr) {
#ifdef _DEBUG
		//ファイルが開けなかったときの処理。
		char message[256];
		sprintf(message, "AnimationClip::Load, ファイルのオープンに失敗しました。%ls\n", filePath);
		MessageBox(NULL, message, "Error", MB_OK);
		//止める。
		std::abort();
#endif
		return;
	}
	
	//アニメーションクリップのヘッダーをロード。
	AnimClipHeader header;
	fread(&header, sizeof(header), 1, fp);
		
	if (header.numAnimationEvent > 0) {
		for (int i = 0; i < header.numAnimationEvent; i++) {
			std::uint32_t eventnum = 0;
			fread(&eventnum, sizeof(eventnum), 1, fp);
			AnimationEvent* Event = new AnimationEvent;
			for (int j = 0; j < eventnum; j++)
			{
				AnimationEventData eventdata;
				//ここでファイルからデータを抽出
				//発生時間と文字の長さのみ
				fread(&eventdata, sizeof(eventdata), 1, fp);
				static char name[255];
				static wchar_t neme_t[255];
				//ここでファイルからデータを抽出
				//文字自体のデータ
				fread(&name, eventdata.eventNameLength + 1, 1, fp);
				//ここでもらったデータをcharをwchar_tに変換を変える
				mbstowcs(neme_t, name, 255);
				//できたデータをまとめる
				auto dete = new EventDate;
				dete->eventNameLength = neme_t;
				dete->invokeTime = eventdata.invokeTime;
				Event->SetEventDete(dete);
				//出来上がったデータをリストに積む
			}
			m_AnimationEventlist.push_back(Event);
		}
	}


	//中身コピーするためのメモリをドカッと確保。
	KeyframeRow* keyframes = new KeyframeRow[header.numKey];
	//キーフレームをドカッと読み込み。
	fread(keyframes, sizeof(KeyframeRow), header.numKey, fp);
	//もうデータのロードはすべて終わったので、ファイルは閉じる。
	fclose(fp);
	//tkaファイルのキーフレームのローカル業レは4x3行列なので
	//ゲームで使用しやすいように、4x4行列に変換していく。
	for (int i = 0; i < (int)header.numKey; i++) {
		//ゲームで使用するKeyframeのインスタンスを生成。
		Keyframe* keyframe = new Keyframe;
		//ボーン番号とか再生時間とかをコピーしていく。
		keyframe->boneIndex = keyframes[i].boneIndex;
		keyframe->transform = CMatrix::Identity();
		keyframe->time = keyframes[i].time;
		//行列はコピー。
		for (int j = 0; j < 4; j++) {
			keyframe->transform.m[j][0] = keyframes[i].transform[j].x;
			keyframe->transform.m[j][1] = keyframes[i].transform[j].y;
			keyframe->transform.m[j][2] = keyframes[i].transform[j].z;
		}
		//新しく作ったキーフレームを可変長配列に追加。
		m_keyframes.push_back(keyframe);
	}

	//キーフレームは全部コピー終わったので、ファイルから読み込んだ分は破棄する。
	delete[] keyframes;

	//ボーン番号ごとにキーフレームを振り分けていく。
	m_keyFramePtrListArray.resize(MAX_BONE);
	for (Keyframe* keyframe : m_keyframes) {
		m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe);
		if (m_topBoneKeyFramList == nullptr) {
			m_topBoneKeyFramList = &m_keyFramePtrListArray[keyframe->boneIndex];
		}
	}
	//最初のボーンと最後のボーンの差を調べる
	Differencetransform.resize(MAX_BONE);
	for (int i = 0; i < m_keyFramePtrListArray.size(); i++) {
		if (m_keyFramePtrListArray[i].size() != 0) {
			for (int j = 0; j < m_keyFramePtrListArray[i].size(); j++) {
				auto nfa = m_keyFramePtrListArray[i][0]->transform.v[3] - m_keyFramePtrListArray[i][j]->transform.v[3];
				CVector3 length = CVector3::Zero();
				length.x = nfa.x;
				length.y = nfa.y;
				//length.z = nfa.z;
				Differencetransform[i].push_back(length);
			}
		}
	}
}

