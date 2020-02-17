#pragma once

#include "Skeleton.h"
#include"SkinModelManager.h"
class StructuredBuffer;
/*!
*@brief	FBXの上方向。
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
*@brief	スキンモデルクラス。
*/
class SkinModel
{
public:

	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();
	
	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, int maxInstance = 1, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);

	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	bool init(const wchar_t* filePath);

	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	void UpdateInstancingData(
		const CVector3& trans,
		const CQuaternion& rot,
		const CVector3& scale
	/*EnFbxUpAxis enUpdateAxis*/);
	/*!
*@brief	インスタンスデータの更新開始時に呼び出してください。
*/
	void BeginUpdateInstancingData();

	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	モデルを描画。
	*@param[in]	viewMatrix		カメラ行列。
	*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
	*@param[in]	projMatrix		プロジェクション行列。
	*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
	*/
	void Draw(EnDrawMode drawMode, CMatrix viewMatrix, CMatrix projMatrix );
	//エンジン内で呼ぶ関数のため使用禁止！！！！
	void Draw(int No);
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
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
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};

	//カリング計算関数
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
		return m_worldMatrix;					//!<ワールド行列。
	}
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState(int maxInstance);
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);

	bool GetPositivePoint(int No, CVector3 pos,const Plane m_kaku[6]);
	bool GetPositivePoint(int No, CVector3 pos, const Plane m_kaku[6], int nn, int sNo);
	//CVector3 GetNegativePoint(int No,CVector3 pos);

private:
	//定数バッファ。
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

	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
	ID3D11Buffer*		m_cb = nullptr;					//!<定数バッファ。
	Skeleton			m_skeleton;						//!<スケルトン。
	CMatrix				m_worldMatrix;					//!<ワールド行列。
	DirectX::Model*		m_modelDx;						//!<DirectXTKが提供するモデルクラス。
	//武器用の変数
	DirectX::Model*		m_modelDxweapon = nullptr;						//!<DirectXTKが提供するモデルクラス。
	bool				m_weapon = false;
	
	
	CVector3 Maxpos = CVector3::Zero();
	CVector3 Minpos = CVector3::Zero();
	float CameraAngle = 0.0f;
	CVector3 m_scale = CVector3::One();
	float m_cameralen = FLT_MAX;
	bool result = false;									//スキンモデルにボーンがあるかどうか

	//bool m_isShadowReciever = false;					//!<シャドーレシーバーにするかのフラグ
	//ここからマルチスレッド関連の加工済みデータ
	OBB m_box;

	static const int MAXTHREAD = 2;
	SVSConstantBuffer m_vsCb[MAXTHREAD];
	SVSConstantBuffer m_vsSCb[MAXTHREAD];
	ID3D11SamplerState* m_samplerState[MAXTHREAD] = { nullptr,nullptr };		//!<サンプラステート。
	bool m_isShadowCaster[MAXTHREAD] = { false };
	EnDrawMode m_Mode[MAXTHREAD] = { enShadow,enShadow };
	//インスタンシング用
	std::vector<CMatrix> m_instancingData[MAXTHREAD];		//!<インスタンシング描画用のデータ。
	std::vector<CVector3> m_instancingScale[MAXTHREAD];		//!<インスタンシングの拡大データ。
	std::vector<CMatrix> m_drawData[2];		//!<インスタンシング描画用のデータ。（表示の数だけ必要）


	//草の動かすように用
	ID3D11Buffer*		m_Nature = nullptr;					//!<定数バッファ。
	float				m_ugoku = 0.0f;


	std::vector<CMatrix> m_Matrix[MAXTHREAD];						//スキンモデル付きのインスタンシング用配列

	int m_numInstance = 0;								//!<インスタンシング用の個数
	int m_maxInstance = 0;							    //!<インスタンシングデータの最大数(これ以上は表示できません)
	StructuredBuffer m_instancingDataSB[2];	    //!<インスタンシング描画用のストラクチャーバッファ。
};