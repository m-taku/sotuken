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
	CMatrix transMatrixMAX, transMatrixMIN;
	//平行移動行列を作成する。
	transMatrixMAX.MakeTranslation(Maxpos);
	transMatrixMAX.Mul(mWorld, transMatrixMAX);
	transMatrixMAX.Mul(transMatrixMAX, m_vsCb[No].mView);
	transMatrixMAX.Mul(transMatrixMAX, m_vsCb[No].mProj);
	transMatrixMAX.m[3][0] /= transMatrixMAX.m[3][3];
	transMatrixMAX.m[3][1] /= transMatrixMAX.m[3][3];
	transMatrixMAX.m[3][2] /= transMatrixMAX.m[3][3];
	transMatrixMIN.MakeTranslation(Minpos);	
	transMatrixMIN.Mul(mWorld, transMatrixMIN);
	transMatrixMIN.Mul(transMatrixMIN, m_vsCb[No].mView);
	transMatrixMIN.Mul(transMatrixMIN, m_vsCb[No].mProj);
	transMatrixMIN.m[3][0] /= transMatrixMIN.m[3][3];
	transMatrixMIN.m[3][1] /= transMatrixMIN.m[3][3];
	transMatrixMIN.m[3][2] /= transMatrixMIN.m[3][3];


	CVector4 vectorMAX = transMatrixMAX.v[3];
	CVector4 vectorMIN = transMatrixMIN.v[3];
	if (vectorMIN.x > 1.0f || vectorMIN.x < -1.0f
		|| vectorMIN.y>1.0f || vectorMIN.y < -1.0f
		|| vectorMIN.z>1.0f || vectorMIN.z < -1.0f
		)
	{
		if (vectorMAX.x > 1.0f || vectorMAX.x < -1.0f
			|| vectorMAX.y>1.0f || vectorMAX.y < -1.0f
			|| vectorMAX.z>1.0f || vectorMAX.z < -1.0f) {

			return false;
		}
	}
	return true;
}