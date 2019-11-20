#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelEffect.h"
#include "SkinModelDataManager.h"

SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//定数バッファを解放。
		m_cb->Release();
	}
	for (int i = 0; i < MAXTHREAD; i++) {
		if (m_samplerState != nullptr) {
			//サンプラステートを解放。
			m_samplerState[i]->Release();
		}
	}

	smLightManager().DeleteSkinModel(this);
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//サンプラステートの初期化。
	InitSamplerState();

	CMatrix mBias;
	mBias.MakeRotationX(CMath::PI * -0.5f);
	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);
	Maxpos.x = FLT_MIN;
	Maxpos.y = FLT_MIN;
	Maxpos.z = FLT_MIN;
	Minpos.x = FLT_MAX;
	Minpos.y = FLT_MAX;
	Minpos.z = FLT_MAX;
	FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//頂点バッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->vertexBuffer->GetDesc(&bufferDesc);
			int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(subresource.pData);
			CVector3 pos;
			for (int i = 0; i < vertexCount; i++) {
				pos = *reinterpret_cast<CVector3*>(pData);
				//バイアスをかける。
				mBias.Mul(pos);
				Maxpos.x = max(pos.x,Maxpos.x);
				Maxpos.y = max(pos.y, Maxpos.y);
				Maxpos.z = max(pos.z, Maxpos.z);
				Minpos.x = min(pos.x, Minpos.x);
				Minpos.y = min(pos.y, Minpos.y);
				Minpos.z = min(pos.z, Minpos.z);
				//次の頂点へ。
				pData += mesh->vertexStride;
			}
			//頂点バッファをアンロック
		}
	});
	//AABBをOBBに
	auto origin = Maxpos - Minpos;
	origin /= 2.0f;
	CVector3 directionX, directionY, directionZ;
	directionX = CVector3::AxisX();
	directionZ = CVector3::AxisX();
	directionZ = CVector3::AxisZ();
	float directionXLen = 0.0f;
	float directionYLen = 0.0f;
	float directionZLen = 0.0f;

	directionXLen = Maxpos.x - origin.x;
	directionYLen = Maxpos.y - origin.y;
	directionZLen = Maxpos.z - origin.z;
	m_atari.origin = origin;
	m_atari.direction[0] = CVector3::AxisX();
	m_atari.direction[1] = CVector3::AxisY();
	m_atari.direction[2] = CVector3::AxisZ();
	m_atari.directionLen.x = directionXLen;	
	m_atari.directionLen.y = directionYLen;
	m_atari.directionLen.z = directionZLen;

	m_enFbxUpAxis = enFbxUpAxis;
	smLightManager().AddSkinModel(this);
}
void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if (result == false) {
		//スケルトンが読み込みに失敗した。
		//アニメーションしないモデルは、スケルトンが不要なので
		//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
	//作成。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
}
void SkinModel::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	for (int i = 0; i < MAXTHREAD; i++) {
		g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState[i]);
	}
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation(position);
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//スケルトンの更新。
	m_skeleton.Update(m_worldMatrix);
}
void SkinModel::Draw(EnDrawMode drawMode, CMatrix viewMatrix, CMatrix projMatrix)
{

	if (drawMode == enShadow) {
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//定数バッファの内容を更新。
		SVSConstantBuffer vsCb;
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		//GetSkinModelManager().SetModel(this);
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		//定数バッファをGPUに転送。
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
		//サンプラステートを設定。
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[0]);
		//ボーン行列をGPUに転送。
		m_skeleton.SendBoneMatrixArrayToGPU();

		FindMesh([&](auto& ef) {
			ModelEffect* effect = (ModelEffect*)ef->effect.get();
			effect->SetDrawMode(drawMode);
		});

		//描画。
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix
		);
	}
	else
	{
		auto No = GetSkinModelManager().GetNo();
		m_vsCb[No].mWorld = m_worldMatrix;
		m_vsCb[No].mProj = projMatrix;
		m_vsCb[No].mView = viewMatrix;
		m_Mode[No] = drawMode;
		GetSkinModelManager().SetModel(this);
	}
}

void SkinModel::Draw(int No)
{
	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

		
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_vsCb[No].mWorld;
	vsCb.mProj = m_vsCb[No].mProj;
	vsCb.mView = m_vsCb[No].mView;
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[No]);
	//ボーン行列をGPUに転送。
	m_skeleton.SendBoneMatrixArrayToGPU();

	FindMesh([&](auto& ef) {
		ModelEffect* effect = (ModelEffect*)ef->effect.get();
		effect->SetDrawMode(m_Mode[No]);
	});

	//描画。
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_vsCb[No].mWorld,
		m_vsCb[No].mView,
		m_vsCb[No].mProj
	);
}
bool SkinModel::Culling(int No)
{
	auto mWorld = m_vsCb[No].mWorld;
	CMatrix transMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation(m_atari.origin);
	transMatrix.Mul(mWorld, transMatrix);
	m_atari.direction[0].x = transMatrix.m[0][0];//右
	m_atari.direction[0].y = transMatrix.m[0][1];
	m_atari.direction[0].z = transMatrix.m[0][2];
	m_atari.direction[1].x = transMatrix.m[1][0];//うｐ
	m_atari.direction[1].y = transMatrix.m[1][1];
	m_atari.direction[1].z = transMatrix.m[1][2];
	m_atari.direction[2].x = transMatrix.m[2][0];//まえ
	m_atari.direction[2].y = transMatrix.m[2][1];
	m_atari.direction[2].z = transMatrix.m[2][2];
	m_atari.origin.x = transMatrix.m[3][0];//まえ
	m_atari.origin.y = transMatrix.m[3][1];
	m_atari.origin.z = transMatrix.m[3][2];
	CVector3 Minpos;
	Minpos = m_atari.origin;
	Minpos -= m_atari.direction[0];
	Minpos -= m_atari.direction[1];
	Minpos -= m_atari.direction[2];
	CalculateFrustumPlanes(m_vsCb[No].mProj,No);

	return false;
}

/// <summary>
/// 指定されたProjection Matricsから視錐台の6面の平面を求める
/// </summary>
/// <param name="pmat">Projection Matrix</param>
/// <param name="eyeTrans">カメラ位置</param>
/// <param name="near">カメラのNear</param>
/// <param name="far">カメラのFar</param>
/// <returns></returns>
void SkinModel::CalculateFrustumPlanes(CMatrix pmat,int No)
{

	auto n = m_vsCb[No].mView;
	// 0: Left, 1: Right, 2: Bottm, 3: Top
	for (int i = 0; i < 4; i++)
	{
		CVector3 m_pos;
		int r = i / 2;
		if (i % 2 == 0)
		{
			// 平面の方程式
			// ax + by + cz + d = 0
			m_pos.x = pmat.m[3][0] - pmat.m[r][0];
			m_pos.y = pmat.m[3][1] - pmat.m[r][1];
			m_pos.z = pmat.m[3][2] - pmat.m[r][2];
			//m_pos.w = pmat.m[3][3] - pmat.m[r][3];
		}
		else
		{
			m_pos.x = pmat.m[3][0] + pmat.m[r][0];
			m_pos.y = pmat.m[3][1] + pmat.m[r][1];
			m_pos.z = pmat.m[3][2] + pmat.m[r][2];
			//m_pos.w = pmat.m[3][3] + pmat.m[r][3];
		}
		CVector3 normal;
		m_pos.Normalize();
		normal = m_pos * -1.0f;
		n.Inverse(n);
		CVector3 kaku;
		kaku.x = n.m[2][0];//まえ
		kaku.y = n.m[2][1];
		kaku.z = n.m[2][2];
		kaku.Normalize();
		CVector3 jiku;
		jiku.Cross(kaku,CVector3::AxisZ());
		float m = kaku.Dot(CVector3::AxisZ());
		CQuaternion na;
		na.SetRotation(jiku,m);
		normal.Normalize();
		na.Multiply(normal);
		CVector3 popopop;
		popopop.x = n.m[3][0];
		popopop.y = n.m[3][1];
		popopop.z = n.m[3][2];
		m_kaku[i].m_normal = normal;
		m_kaku[i].m_popopop = popopop;
	}
}
