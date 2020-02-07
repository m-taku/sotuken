/*!
 *@brief	�A�j���[�V�����N���b�v�B
 */

#include "stdafx.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"


AnimationClip::~AnimationClip()
{
	//�L�[�t���[����j���B
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
		//�t�@�C�����J���Ȃ������Ƃ��̏����B
		char message[256];
		sprintf(message, "AnimationClip::Load, �t�@�C���̃I�[�v���Ɏ��s���܂����B%ls\n", filePath);
		MessageBox(NULL, message, "Error", MB_OK);
		//�~�߂�B
		std::abort();
#endif
		return;
	}
	
	//�A�j���[�V�����N���b�v�̃w�b�_�[�����[�h�B
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
				//�����Ńt�@�C������f�[�^�𒊏o
				//�������Ԃƕ����̒����̂�
				fread(&eventdata, sizeof(eventdata), 1, fp);
				static char name[255];
				static wchar_t neme_t[255];
				//�����Ńt�@�C������f�[�^�𒊏o
				//�������̂̃f�[�^
				fread(&name, eventdata.eventNameLength + 1, 1, fp);
				//�����ł�������f�[�^��char��wchar_t�ɕϊ���ς���
				mbstowcs(neme_t, name, 255);
				//�ł����f�[�^���܂Ƃ߂�
				auto dete = new EventDate;
				dete->eventNameLength = neme_t;
				dete->invokeTime = eventdata.invokeTime;
				Event->SetEventDete(dete);
				//�o���オ�����f�[�^�����X�g�ɐς�
			}
			m_AnimationEventlist.push_back(Event);
		}
	}


	//���g�R�s�[���邽�߂̃��������h�J�b�Ɗm�ہB
	KeyframeRow* keyframes = new KeyframeRow[header.numKey];
	//�L�[�t���[�����h�J�b�Ɠǂݍ��݁B
	fread(keyframes, sizeof(KeyframeRow), header.numKey, fp);
	//�����f�[�^�̃��[�h�͂��ׂďI������̂ŁA�t�@�C���͕���B
	fclose(fp);
	//tka�t�@�C���̃L�[�t���[���̃��[�J���ƃ���4x3�s��Ȃ̂�
	//�Q�[���Ŏg�p���₷���悤�ɁA4x4�s��ɕϊ����Ă����B
	for (int i = 0; i < (int)header.numKey; i++) {
		//�Q�[���Ŏg�p����Keyframe�̃C���X�^���X�𐶐��B
		Keyframe* keyframe = new Keyframe;
		//�{�[���ԍ��Ƃ��Đ����ԂƂ����R�s�[���Ă����B
		keyframe->boneIndex = keyframes[i].boneIndex;
		keyframe->transform = CMatrix::Identity();
		keyframe->time = keyframes[i].time;
		//�s��̓R�s�[�B
		for (int j = 0; j < 4; j++) {
			keyframe->transform.m[j][0] = keyframes[i].transform[j].x;
			keyframe->transform.m[j][1] = keyframes[i].transform[j].y;
			keyframe->transform.m[j][2] = keyframes[i].transform[j].z;
		}
		//�V����������L�[�t���[�����ϒ��z��ɒǉ��B
		m_keyframes.push_back(keyframe);
	}

	//�L�[�t���[���͑S���R�s�[�I������̂ŁA�t�@�C������ǂݍ��񂾕��͔j������B
	delete[] keyframes;

	//�{�[���ԍ����ƂɃL�[�t���[����U�蕪���Ă����B
	m_keyFramePtrListArray.resize(MAX_BONE);
	for (Keyframe* keyframe : m_keyframes) {
		m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe);
		if (m_topBoneKeyFramList == nullptr) {
			m_topBoneKeyFramList = &m_keyFramePtrListArray[keyframe->boneIndex];
		}
	}
	//�ŏ��̃{�[���ƍŌ�̃{�[���̍��𒲂ׂ�
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

