#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelEffect.h"
#include "SkinModelDataManager.h"
SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//�萔�o�b�t�@������B
		m_cb->Release();
	}
	for (int i = 0; i < MAXTHREAD; i++) {
		if (m_samplerState != nullptr) {
			//�T���v���X�e�[�g������B
			m_samplerState[i]->Release();
		}
	}

	smLightManager().DeleteSkinModel(this);
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	InitSkeleton(filePath);

	//�萔�o�b�t�@�̍쐬�B
	InitConstantBuffer();

	//�T���v���X�e�[�g�̏������B
	InitSamplerState();

	CMatrix mBias;
	mBias.MakeRotationX(CMath::PI * -0.5f);
	//SkinModelDataManager���g�p����CMO�t�@�C���̃��[�h�B
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);
	Maxpos.x = FLT_MIN;
	Maxpos.y = FLT_MIN;
	Maxpos.z = FLT_MIN;
	Minpos.x = FLT_MAX;
	Minpos.y = FLT_MAX;
	Minpos.z = FLT_MAX;
	FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//���_�o�b�t�@���쐬�B
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
				//�o�C�A�X��������B
				mBias.Mul(pos);
				Maxpos.x = max(pos.x,Maxpos.x);
				Maxpos.y = max(pos.y, Maxpos.y);
				Maxpos.z = max(pos.z, Maxpos.z);
				Minpos.x = min(pos.x, Minpos.x);
				Minpos.y = min(pos.y, Minpos.y);
				Minpos.z = min(pos.z, Minpos.z);
				//���̒��_�ցB
				pData += mesh->vertexStride;
			}
			//���_�o�b�t�@���A�����b�N
		}
	});
	//AABB��OBB��
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

	m_enFbxUpAxis = enFbxUpAxis;
	smLightManager().AddSkinModel(this);
}
void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	//cmo�t�@�C���̊g���q��tks�ɕύX����B
	std::wstring skeletonFilePath = filePath;
	//�����񂩂�.cmo�t�@�C���n�܂�ꏊ�������B
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmo�t�@�C����.tks�ɒu��������B
	skeletonFilePath.replace(pos, 4, L".tks");
	//tks�t�@�C�������[�h����B
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if (result == false) {
		//�X�P���g�����ǂݍ��݂Ɏ��s�����B
		//�A�j���[�V�������Ȃ����f���́A�X�P���g�����s�v�Ȃ̂�
		//�ǂݍ��݂Ɏ��s���邱�Ƃ͂���̂ŁA���O�o�͂����ɂ��Ă����B
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tks�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(SVSConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	//�쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
}
void SkinModel::InitSamplerState()
{
	//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
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
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//���s�ړ��s����쐬����B
	transMatrix.MakeTranslation(position);
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion(rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	//�g��s����쐬����B
	scaleMatrix.MakeScaling(scale);
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//�X�P���g���̍X�V�B
	m_skeleton.Update(m_worldMatrix);
}
void SkinModel::Draw(EnDrawMode drawMode, CMatrix viewMatrix, CMatrix projMatrix)
{

	if (drawMode == enShadow) {
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//�萔�o�b�t�@�̓��e���X�V�B
		SVSConstantBuffer vsCb;
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		//GetSkinModelManager().SetModel(this);
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		//�萔�o�b�t�@��GPU�ɓ]���B
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
		//�T���v���X�e�[�g��ݒ�B
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[0]);
		//�{�[���s���GPU�ɓ]���B
		m_skeleton.SendBoneMatrixArrayToGPU();

		FindMesh([&](auto& ef) {
			ModelEffect* effect = (ModelEffect*)ef->effect.get();
			effect->SetDrawMode(enNormal/*drawMode*/);
		});

		//�`��B
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
	//�萔�o�b�t�@��GPU�ɓ]���B
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//�T���v���X�e�[�g��ݒ�B
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[No]);
	//�{�[���s���GPU�ɓ]���B
	m_skeleton.SendBoneMatrixArrayToGPU();

	FindMesh([&](auto& ef) {
		ModelEffect* effect = (ModelEffect*)ef->effect.get();
		effect->SetDrawMode(m_Mode[No]);
	});

	//�`��B
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
	//���s�ړ��s����쐬����B
	//CMatrix mBias = CMatrix::Identity();
	//mBias.MakeRotationX(CMath::PI * -0.5f);
	//mBias.Inverse(mBias);
	transMatrix.MakeTranslation(m_atari.origin);
	//transMatrix.Mul(mBias, transMatrix);
	//transMatrix.Mul(mBias, transMatrix);
	transMatrix.Mul(mWorld, transMatrix);
	m_atari.direction[0].x = transMatrix.m[0][0];//�E
	m_atari.direction[0].y = transMatrix.m[0][1];
	m_atari.direction[0].z = transMatrix.m[0][2];
	m_atari.direction[1].x = transMatrix.m[1][0];//����
	m_atari.direction[1].y = transMatrix.m[1][1];
	m_atari.direction[1].z = transMatrix.m[1][2];
	m_atari.direction[2].x = transMatrix.m[2][0];//�܂�
	m_atari.direction[2].y = transMatrix.m[2][1];
	m_atari.direction[2].z = transMatrix.m[2][2];
	m_atari.direction[0].Normalize();
	m_atari.direction[1].Normalize();
	m_atari.direction[2].Normalize();

	CQuaternion na;
	na.SetRotationDeg(CVector3::AxisX(),90.0f);
	m_atari.direction[0];
	na.Multiply(m_atari.direction[0]);
	na.Multiply(m_atari.direction[1]);
	na.Multiply(m_atari.direction[2]);
	m_atari.direction[0].Normalize();
	m_atari.direction[1].Normalize();
	m_atari.direction[2].Normalize();
	CVector3 ni;
	ni.x = transMatrix.m[3][0];//�܂�
	ni.y = transMatrix.m[3][1];
	ni.z = transMatrix.m[3][2];
	CVector3 Minposa;
	Minposa = ni;
	Minposa -= m_atari.direction[0] * m_atari.directionLen.x;
	Minposa -= m_atari.direction[1] * m_atari.directionLen.y;
	Minposa -= m_atari.direction[2] * m_atari.directionLen.z;
	CalculateFrustumPlanes(m_vsCb[No].mProj,No);

	for (int i = 0; i < 4; i++) {
		CVector3 Max = GetPositivePoint(i, Minposa);
		//CVector3 Min = GetNegativePoint(No, Minpos);


		// (vp - plane.pos)�Enormal
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

	//float dn = planes[i].GetDistanceToPoint(vn);
	//if (dn < 0)
	//{
	//	result = State.Intersect;
	//}


}
/// <summary>
/// �@�������ԋ߂��_���Z�o����
/// </summary>
/// <param name="target">�^�[�Q�b�g�ƂȂ�AABB</param>
/// <param name="normal">�Z�o����@��</param>
/// <returns></returns>
CVector3 SkinModel::GetPositivePoint(int No, CVector3 pos)
{
	CVector3 pos12 = m_kaku[No].m_popopop;
	float len = FLT_MAX;
	CVector3 bb = pos;
	CVector3 bb211 = CVector3::Zero();
	for (int i = 0; i < 7; i++)
	{
		auto na = m_kaku[No].m_normal.Dot(bb - m_kaku[No].m_popopop); ;
		if (0 < na)
		{
			len = fabsf(na);
			bb211 = bb;
			return bb211;
		}
		if (i % 2 == 0) {
			//X�̓���
			float fugou = 1.0f;
			if (i / 2 % 2 !=0)
			{
				fugou *= -1.0f;
			}
			bb += m_atari.direction[0] * m_atari.directionLen.x*( 2.0f*fugou);
		}
		if ((i + 3) % 4 == 0)
		{
			float fugou = 1.0f;
			if ((i+3) / 4 % 2 == 0)
			{
				fugou *= -1.0f;
			}
			bb += m_atari.direction[1] * m_atari.directionLen.y * (2.0f*fugou);
		}
		if (i == 3)
		{
			bb += m_atari.direction[2] * m_atari.directionLen.z * 2.0f;

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
///// �@�������ԉ����_���Z�o����
///// </summary>
///// <param name="target">�^�[�Q�b�g�ƂȂ�AABB</param>
///// <param name="normal">�Z�o����@��</param>
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
/// �w�肳�ꂽProjection Matrics���王�����6�ʂ̕��ʂ����߂�
/// </summary>
/// <param name="pmat">Projection Matrix</param>
/// <returns></returns>
void SkinModel::CalculateFrustumPlanes(CMatrix pmat,int No)
{

	auto n = m_vsCb[No].mView;
	n.Inverse(n);
	// 0: Left, 1: Right, 2: Bottm, 3: Top
	for (int i = 0; i < 4; i++)
	{
		CVector3 m_pos;
		int r = i / 2;
		if (i % 2 == 0)
		{
			// ���ʂ̕�����
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
		CVector3 kaku,rite;
		kaku.x = n.m[2][0];//�܂�
		kaku.y = n.m[2][1];
		kaku.z = n.m[2][2];
		kaku.Normalize();
		CVector3 popopop;
		popopop.x = n.m[3][0];
		popopop.y = n.m[3][1];
		popopop.z = n.m[3][2];

		//rite.x = n.m[0][0];//�E
		//rite.y = n.m[0][1];
		//rite.z = n.m[0][2];
		//rite.Normalize();

		float aspect = FRAME_BUFFER_W / FRAME_BUFFER_H;


		//CVector3 GameCamUp;
		//GameCamUp.Cross(kaku, rite);
		//GameCamUp.Normalize();

		//CVector3 GameCamRight = smGameCamera().GetCameraRight();

		CVector3 normal;
		m_pos.Normalize();
		normal = m_pos * -1.0f;
		//CVector3 kaku;
		kaku.x = n.m[2][0];//�܂�
		kaku.y = n.m[2][1];
		kaku.z = n.m[2][2];
		kaku.Normalize();
		//kaku.y = 0.0f;
		CVector3 jiku;
		auto hajiki = kaku.Dot(CVector3::AxisZ());
		if (fabsf(hajiki) < 0.99999f) {
			jiku.Cross(CVector3::AxisZ(), kaku);
		}
		else
		{
			
			//jiku = CVector3::AxisY();
		}
		CQuaternion na;
		//if (hajiki > 0.0f || hajiki < -FLT_MIN)
		//{
		//	hajiki = CMath::RadToDeg(hajiki);
		//	//CVector3 jiku;
		//	jiku.Cross(CVector3::AxisZ(), kaku);
		//	//if (jiku.y > 0.0f || jiku.y < -FLT_MIN)
		//	{
		//		jiku.Normalize();
		//		na.SetRotationDeg(jiku, hajiki);
		//	}
		//}
		float m = acosf(min(1.0f,max(-1.0f,kaku.Dot(CVector3::AxisZ()))));		
		auto naagreagaeg = CMath::RadToDeg(m);
		
		
		jiku.Normalize();
		normal.Normalize();

		//normal.z -= 1.0f;
	//	CVector3 popopop;
		popopop.x = n.m[3][0];
		popopop.y = n.m[3][1];
		popopop.z = n.m[3][2];
		m_kaku[i].devud_Nomal = normal;
		na.SetRotationDeg(jiku, naagreagaeg);
		na.Multiply(normal);
		normal.Normalize();
		//normal = kaku + hiki;
		m_kaku[i].devud_front = kaku;
		m_kaku[i].m_normal = normal;
		m_kaku[i].m_popopop = popopop;

	}
}
