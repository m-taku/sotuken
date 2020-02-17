#pragma once

#include "Skeleton.h"
#include"SkinModelManager.h"
class StructuredBuffer;
/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
enum EnDrawMode {
	enNormal,
	enTree,
	enTreeInstancing,
	enInstancing,
	enShadow, 
	enTreeShadow,
	enTreeShadowInstancing,
	enInstancingShadow,
	enCubeMap
};

struct OBB
{
	CVector3 origin = CVector3::Zero();
	CVector3 direction[3] = { CVector3::Zero() };
	CVector3 directionLen = CVector3::Zero();
};

/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel
{
public:

	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();
	
	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, int maxInstance = 1, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);

	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	bool init(const wchar_t* filePath);

	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	void UpdateInstancingData(
		const CVector3& trans,
		const CQuaternion& rot,
		const CVector3& scale
	/*EnFbxUpAxis enUpdateAxis*/);
	/*!
*@brief	�C���X�^���X�f�[�^�̍X�V�J�n���ɌĂяo���Ă��������B
*/
	void BeginUpdateInstancingData();

	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	���f����`��B
	*@param[in]	viewMatrix		�J�����s��B
	*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
	*@param[in]	projMatrix		�v���W�F�N�V�����s��B
	*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
	*/
	void Draw(EnDrawMode drawMode, CMatrix viewMatrix, CMatrix projMatrix );
	//�G���W�����ŌĂԊ֐��̂��ߎg�p�֎~�I�I�I�I
	void Draw(int No);
	/*!
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}

	bool IsShadowCaster()
	{
		return m_isShadowCaster[0];
	}

	void EnableShadowCaster(bool frag)
	{
		m_isShadowCaster[0] = frag;
	}

	/*!
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};

	//�J�����O�v�Z�֐�
	bool Culling(EnDrawMode drawMode, int No,const Plane m_kaku[6]);
	void CullingInstancing(EnDrawMode drawMode, int No,const Plane m_kaku[6]);

	float Getcameralen()
	{
		return m_cameralen;
	}
	void Setweapon(DirectX::Model* modeldata)
	{
		m_modelDxweapon = modeldata;
		m_weapon = true;
	}

	CMatrix GetworldMatrix()
	{
		return m_worldMatrix;					//!<���[���h�s��B
	}
private:
	/*!
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState(int maxInstance);
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);

	bool GetPositivePoint(int No, CVector3 pos,const Plane m_kaku[6]);
	bool GetPositivePoint(int No, CVector3 pos, const Plane m_kaku[6], int nn, int sNo);
	//CVector3 GetNegativePoint(int No,CVector3 pos);

private:
	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
	};
	struct NatureConstantBuffer {
		CVector3 mShakePower;
		float nowPower;
		float maxpos;
	};

	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;					//!<�萔�o�b�t�@�B
	Skeleton			m_skeleton;						//!<�X�P���g���B
	CMatrix				m_worldMatrix;					//!<���[���h�s��B
	DirectX::Model*		m_modelDx;						//!<DirectXTK���񋟂��郂�f���N���X�B
	//����p�̕ϐ�
	DirectX::Model*		m_modelDxweapon = nullptr;						//!<DirectXTK���񋟂��郂�f���N���X�B
	bool				m_weapon = false;
	
	
	CVector3 Maxpos = CVector3::Zero();
	CVector3 Minpos = CVector3::Zero();
	float CameraAngle = 0.0f;
	CVector3 m_scale = CVector3::One();
	float m_cameralen = FLT_MAX;
	bool result = false;									//�X�L�����f���Ƀ{�[�������邩�ǂ���

	//bool m_isShadowReciever = false;					//!<�V���h�[���V�[�o�[�ɂ��邩�̃t���O
	//��������}���`�X���b�h�֘A�̉��H�ς݃f�[�^
	OBB m_box;

	static const int MAXTHREAD = 2;
	SVSConstantBuffer m_vsCb[MAXTHREAD];
	SVSConstantBuffer m_vsSCb[MAXTHREAD];
	ID3D11SamplerState* m_samplerState[MAXTHREAD] = { nullptr,nullptr };		//!<�T���v���X�e�[�g�B
	bool m_isShadowCaster[MAXTHREAD] = { false };
	EnDrawMode m_Mode[MAXTHREAD] = { enShadow,enShadow };
	//�C���X�^���V���O�p
	std::vector<CMatrix> m_instancingData[MAXTHREAD];		//!<�C���X�^���V���O�`��p�̃f�[�^�B
	std::vector<CVector3> m_instancingScale[MAXTHREAD];		//!<�C���X�^���V���O�̊g��f�[�^�B
	std::vector<CMatrix> m_drawData[2];		//!<�C���X�^���V���O�`��p�̃f�[�^�B�i�\���̐������K�v�j


	//���̓������悤�ɗp
	ID3D11Buffer*		m_Nature = nullptr;					//!<�萔�o�b�t�@�B
	float				m_ugoku = 0.0f;


	std::vector<CMatrix> m_Matrix[MAXTHREAD];						//�X�L�����f���t���̃C���X�^���V���O�p�z��

	int m_numInstance = 0;								//!<�C���X�^���V���O�p�̌�
	int m_maxInstance = 0;							    //!<�C���X�^���V���O�f�[�^�̍ő吔(����ȏ�͕\���ł��܂���)
	StructuredBuffer m_instancingDataSB[2];	    //!<�C���X�^���V���O�`��p�̃X�g���N�`���[�o�b�t�@�B
};