#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelEffect.h"
#include "SkinModelDataManager.h"
#include"StructuredBuffer.h"
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
	for (int i = 0; i < MAXTHREAD; i++) {
		m_instancingDataSB[i].Release();
	}
	smLightManager().DeleteSkinModel(this);
}
void SkinModel::Init(const wchar_t* filePath, int maxInstance, EnFbxUpAxis enFbxUpAxis)
{
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//サンプラステートの初期化。
	InitSamplerState(maxInstance);

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
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	FindMesh([&](const auto& mesh) {
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
				Maxpos.x = max(pos.x, Maxpos.x);
				Maxpos.y = max(pos.y, Maxpos.y);
				Maxpos.z = max(pos.z, Maxpos.z);
				Minpos.x = min(pos.x, Minpos.x);
				Minpos.y = min(pos.y, Minpos.y);
				Minpos.z = min(pos.z, Minpos.z);
				//次の頂点へ。
				pData += mesh->vertexStride;
			}
			//頂点バッファをアンロック
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
		}
	});
	//AABBをOBBに
	auto origin = Maxpos - Minpos;
	origin /= 2.0f;
	origin += Minpos;
	CVector3 directionX, directionY, directionZ;
	directionX = CVector3::AxisX();
	directionZ = CVector3::AxisX();
	directionZ = CVector3::AxisZ();
	float directionXLen = 0.0f;
	float directionYLen = 0.0f;
	float directionZLen = 0.0f;

	directionXLen = (fabsf(Maxpos.x) + fabsf(Minpos.x)) / 2.0f;
	directionYLen = (fabsf(Maxpos.y) + fabsf(Minpos.y)) / 2.0f;
	directionZLen = (fabsf(Maxpos.z) + fabsf(Minpos.z)) / 2.0f;
	m_atari.origin = origin;
	m_atari.direction[0] = CVector3::AxisX();
	m_atari.direction[1] = CVector3::AxisY();
	m_atari.direction[2] = CVector3::AxisZ();
	m_atari.directionLen.x = directionXLen;	
	m_atari.directionLen.y = directionYLen;
	m_atari.directionLen.z = directionZLen;



	CameraAngle = g_camera3D.GetViewAngle();
	m_enFbxUpAxis = enFbxUpAxis;
	//if (maxInstance <= 1) {
	smLightManager().AddSkinModel(this);
	//}
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
	result = m_skeleton.Load(skeletonFilePath.c_str());
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
void SkinModel::InitSamplerState(int maxInstance)
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
		//インスタンシング用のデータを作成。
		if (maxInstance > 1) {
			m_instancingData[i].resize(maxInstance);
			m_Matrix[i].resize(maxInstance);
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRVとしてバインド可能。
			desc.ByteWidth = sizeof(CMatrix) * maxInstance;
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.StructureByteStride = sizeof(CMatrix);
			m_instancingDataSB[i].Create(m_instancingData[i].data(), desc);
			m_maxInstance = maxInstance;
			m_instancingData->clear();
		}
	}

}
void SkinModel::UpdateInstancingData(
	const CVector3& trans,
	const CQuaternion& rot,
	const CVector3& scale
/*EnFbxUpAxis enUpdateAxis*/)
{
	UpdateWorldMatrix(trans, rot, scale);
	if (m_numInstance < m_maxInstance) {
		auto No = GetSkinModelManager().GetNo();
		m_instancingData[No].push_back(m_worldMatrix);
		if (result) {
			//3dsMaxと軸を合わせるためのバイアス。
			CMatrix mBias = CMatrix::Identity();
			if (m_enFbxUpAxis == enFbxUpAxisZ) {
				//Z-upttt
				mBias.MakeRotationX(CMath::PI * -0.5f);
			}
			//スキンあり用の行列を生成
			CMatrix transMatrix, rotMatrix, scaleMatrix;
			//平行移動行列を作成する。
			transMatrix.MakeTranslation(trans);
			//回転行列を作成する。
			rotMatrix = CMatrix::Identity();// MakeRotationFromQuaternion(rot);
			//拡大行列を作成する。
			scaleMatrix = CMatrix::Identity();
			//ワールド行列を作成する。
			//拡大×回転×平行移動の順番で乗算するように！
			//順番を間違えたら結果が変わるよ。
			m_worldMatrix = CMatrix::Identity();
			m_worldMatrix.Mul(scaleMatrix, rotMatrix);
			m_worldMatrix.Mul(m_worldMatrix, transMatrix);
			//m_instancingData[No][m_numInstance] = m_worldMatrix;
			m_worldMatrix.Inverse(m_worldMatrix);
			for (int i = 0; i < m_numInstance; i++) {
				m_Matrix[No][i] = CMatrix::Identity();
				m_Matrix[No][i].Mul(m_instancingData[No][i], m_worldMatrix);
			}
		}
		m_numInstance++;
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
	m_scale = scale;
	scaleMatrix.MakeScaling(m_scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

}
void SkinModel::Draw(EnDrawMode drawMode, CMatrix viewMatrix, CMatrix projMatrix)
{

	auto No = GetSkinModelManager().GetNo();
	if (drawMode == enShadow) {
		m_vsSCb[No].mWorld = m_worldMatrix;
		m_vsSCb[No].mProj = projMatrix;
		m_vsSCb[No].mView = viewMatrix;
		auto delayNo = No;
		delayNo++;
		delayNo %= 2;
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
		if (m_numInstance > 1) {
			//インスタンシング用のデータを更新。
			if (result)
			{
				m_Matrix[delayNo][m_maxInstance - 1] = CMatrix::Identity();
				for (int i = 0; i < m_maxInstance; i++)
				{
					m_instancingData[delayNo][i] = m_Matrix[delayNo][i];
				}
			}
			d3dDeviceContext->UpdateSubresource(m_instancingDataSB[delayNo].GetBody(), 0, NULL, m_drawData[0].data(), 0, 0);
			d3dDeviceContext->VSSetShaderResources(100, 1, &(m_instancingDataSB[delayNo].GetSRV()).GetBody());
		}
		//定数バッファの内容を更新。
		SVSConstantBuffer vsCb;
		if (result) {
			vsCb.mWorld = CMatrix::Identity();
		}
		else
		{
			vsCb.mWorld = m_vsSCb[delayNo].mWorld;
		}
		vsCb.mProj = m_vsSCb[No].mProj;
		vsCb.mView = m_vsSCb[No].mView;
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		//定数バッファをGPUに転送。
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
		//サンプラステートを設定。
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[delayNo]);
		//ボーン行列をGPUに転送。
		m_skeleton.Update(m_vsSCb[delayNo].mWorld);
		m_skeleton.SendBoneMatrixArrayToGPU();

		FindMesh([&](auto& ef) {
			ModelEffect* effect = (ModelEffect*)ef->effect.get();
			effect->SetDrawMode(drawMode);
			if (m_numInstance > 1)
			{
				effect->SetDrawMode(enInstancingShadow);
			}
		});
		//描画。
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_vsSCb[delayNo].mWorld,
			m_vsSCb[No].mView,
			m_vsSCb[No].mProj,
			false,
			nullptr,
			m_numInstance > 1 ? m_numInstance : 1
		);

	}
	else
	{
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
	if (m_maxInstance > 1) {
		//インスタンシング用のデータを更新。
		if (result)
		{
			m_Matrix[No][m_maxInstance - 1] = CMatrix::Identity();
			for (int i = 0; i < m_maxInstance; i++)
			{
				m_instancingData[No][i] = m_Matrix[No][i];
			}
		}
		d3dDeviceContext->UpdateSubresource(m_instancingDataSB[No].GetBody(), 0, NULL, m_drawData[1].data(), 0, 0);
		d3dDeviceContext->VSSetShaderResources(100, 1, &(m_instancingDataSB[No].GetSRV()).GetBody());
	}
		
	SVSConstantBuffer vsCb;
	//定数バッファの内容を更新。
	if (result) {
		vsCb.mWorld = CMatrix::Identity();
	}
	else
	{
		vsCb.mWorld = m_vsCb[No].mWorld;
	}
	vsCb.mProj = m_vsCb[No].mProj;
	vsCb.mView = m_vsCb[No].mView;
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[No]);
	//ボーン行列をGPUに転送。
	//スケルトンの更新。
	m_skeleton.Update(m_vsCb[No].mWorld);
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
		m_vsCb[No].mProj,
		false,
		nullptr,
		m_numInstance > 1 ? m_numInstance : 1
	);
}
void SkinModel::BeginUpdateInstancingData(){
	m_numInstance = 0;
	auto No = GetSkinModelManager().GetNo();
	m_instancingData[No].clear();
}
void SkinModel::CullingInstancing(EnDrawMode drawMode, int No,const sikaku m_kaku[6])
{
	int hoge = 0;
	if (drawMode != enShadow)
	{
		hoge = 1;
	}
	m_drawData[hoge] = (m_instancingData[No]);
	auto data = m_drawData[hoge].begin();
	while(data != m_drawData[hoge].end()) {
		auto mWorld = (*data);
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
		m_atari.direction[0].Normalize();
		m_atari.direction[1].Normalize();
		m_atari.direction[2].Normalize();

		CQuaternion na;
		na.SetRotationDeg(CVector3::AxisX(), 90.0f);
		m_atari.direction[0];
		na.Multiply(m_atari.direction[0]);
		na.Multiply(m_atari.direction[1]);
		na.Multiply(m_atari.direction[2]);
		m_atari.direction[0].Normalize();
		m_atari.direction[1].Normalize();
		m_atari.direction[2].Normalize();
		CVector3 ni;
		ni.x = transMatrix.m[3][0];//まえ
		ni.y = transMatrix.m[3][1];
		ni.z = transMatrix.m[3][2];
		CVector3 Minposa;
		Minposa = ni;
		Minposa -= m_atari.direction[0] * m_atari.directionLen.x * m_scale.x;
		Minposa -= m_atari.direction[1] * m_atari.directionLen.y * m_scale.y;
		Minposa -= m_atari.direction[2] * m_atari.directionLen.z * m_scale.z;
		//CalculateFrustumPlanes(m_vsCb[No].mProj, No);
		m_cameralen = (ni - m_kaku[0].m_popopop).Length();
		int count = 0;
		for (int j = 0; j < 6; j++) {
			CVector3 Max = GetPositivePoint(j, Minposa, m_kaku);
			count++;
			if (Max.Length() == 0)
			{
				data = m_drawData[hoge].erase(data);
				//*data = CMatrix::Identity();
				//m_Matrix[No].erase(data);
				//data++;
				break;
			}
		}	
		if (data == m_drawData[hoge].end())
		{
			break;
		}
		if (count >= 6) {

			data++;						//それ以外は次へ。
		}

	}
}
bool SkinModel::Culling(EnDrawMode drawMode, int No,const sikaku m_kaku[6])
{
	if (m_numInstance < 1){
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
		m_atari.direction[0].Normalize();
		m_atari.direction[1].Normalize();
		m_atari.direction[2].Normalize();

		CQuaternion na;
		na.SetRotationDeg(CVector3::AxisX(), 90.0f);
		m_atari.direction[0];
		na.Multiply(m_atari.direction[0]);
		na.Multiply(m_atari.direction[1]);
		na.Multiply(m_atari.direction[2]);
		m_atari.direction[0].Normalize();
		m_atari.direction[1].Normalize();
		m_atari.direction[2].Normalize();
		CVector3 ni;
		ni.x = transMatrix.m[3][0];//まえ
		ni.y = transMatrix.m[3][1];
		ni.z = transMatrix.m[3][2];
		CVector3 Minposa;
		Minposa = ni;
		Minposa -= m_atari.direction[0] * m_atari.directionLen.x * m_scale.x;
		Minposa -= m_atari.direction[1] * m_atari.directionLen.y * m_scale.y;
		Minposa -= m_atari.direction[2] * m_atari.directionLen.z * m_scale.z;
		//CalculateFrustumPlanes(m_vsCb[No].mProj, No);
		m_cameralen = (ni - m_kaku[0].m_popopop).Length();
		for (int i = 0; i < 6; i++) {
			CVector3 Max = GetPositivePoint(i, Minposa, m_kaku);
			//CVector3 Min = GetNegativePoint(No, Minpos);
			// (vp - plane.pos)・normal
			if (Max.Length() != 0) {
				float dp = m_kaku[i].m_normal.Dot(Max - m_kaku[i].m_popopop);// planes[i].GetDistanceToPoint(vp);
				if (dp < 0)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		if (m_instancingData[No].size() > 0)
		{
			CullingInstancing(drawMode,No, m_kaku);
			if (m_instancingData[No].size() > 0)
			{
				return true;
			}
		}
		return false;
	}
}
/// <summary>
/// 法線から一番近い点を算出する
/// </summary>
/// <param name="target">ターゲットとなるAABB</param>
/// <param name="normal">算出する法線</param>
/// <returns></returns>
CVector3 SkinModel::GetPositivePoint(int No, CVector3 pos,const sikaku m_kaku[6])
{
	CVector3 pos12 = m_kaku[No].m_popopop;
	float len = FLT_MAX;
	CVector3 bb = pos;
	CVector3 bb211 = CVector3::Zero();
	for (int i = 0; i < 7; i++)
	{
		auto na = m_kaku[No].m_normal.Dot(bb - m_kaku[No].m_popopop); 
		if (0 < na)
		{
			len = fabsf(na);
			bb211 = bb;
			return bb211;
		}
		if (i % 2 == 0) {
			//Xの動き
			float fugou = 1.0f;
			if (i / 2 % 2 !=0)
			{
				fugou *= -1.0f;
			}
			bb += m_atari.direction[0] * m_atari.directionLen.x*( 2.0f*fugou)* m_scale.x;
		}
		if ((i + 3) % 4 == 0)
		{
			float fugou = 1.0f;
			if ((i+3) / 4 % 2 == 0)
			{
				fugou *= -1.0f;
			}
			bb += m_atari.direction[1] * m_atari.directionLen.y * (2.0f*fugou)* m_scale.y;
		}
		if (i == 3)
		{
			bb += m_atari.direction[2] * m_atari.directionLen.z * 2.0f* m_scale.z;

		}
	}

	return bb211;
	//if (normal.x > 0)
	//{
	//	pos += m_atari.direction[0] * m_atari.directionLen.x*2.0f;
	//}
	//if (normal.y > 0)
	//{
	//	pos += m_atari.direction[1] * m_atari.directionLen.y*2.0f;
	//}
	//if (normal.z > 0)
	//{
	//	pos += m_atari.direction[2] * m_atari.directionLen.z*2.0f;
	//}

	//return pos;
}


///// <summary>
///// 法線から一番遠い点を算出する
///// </summary>
///// <param name="target">ターゲットとなるAABB</param>
///// <param name="normal">算出する法線</param>
///// <returns></returns>
//CVector3 SkinModel::GetNegativePoint(int No, CVector3 pos)
//{
//	CVector3 normal = m_kaku[No].m_normal;
//
//	if (normal.x < 0)
//	{
//		pos += m_atari.direction[0] * m_atari.directionLen.x;
//	}
//	if (normal.y < 0)
//	{
//		pos += m_atari.direction[1] * m_atari.directionLen.y;
//	}
//	if (normal.z < 0)
//	{
//		pos += m_atari.direction[2] * m_atari.directionLen.z;
//	}
//
//	return pos;
//}
/// <summary>
/// 指定されたProjection Matricsから視錐台の6面の平面を求める
/// </summary>
/// <param name="pmat">Projection Matrix</param>
/// <returns></returns>

