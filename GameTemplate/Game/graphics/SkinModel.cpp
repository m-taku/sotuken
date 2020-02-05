#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelEffect.h"
#include "SkinModelDataManager.h"
#include"StructuredBuffer.h"
SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//�萔�o�b�t�@������B
		m_cb->Release();
	}
	for (int i = 0; i < MAXTHREAD; i++) {
		if (m_samplerState[i] != nullptr) {
			//�T���v���X�e�[�g������B
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
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	InitSkeleton(filePath);

	//�萔�o�b�t�@�̍쐬�B
	InitConstantBuffer();

	//�T���v���X�e�[�g�̏������B
	InitSamplerState(maxInstance);

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
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	FindMesh([&](const auto& mesh) {
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
				Maxpos.x = max(pos.x, Maxpos.x);
				Maxpos.y = max(pos.y, Maxpos.y);
				Maxpos.z = max(pos.z, Maxpos.z);
				Minpos.x = min(pos.x, Minpos.x);
				Minpos.y = min(pos.y, Minpos.y);
				Minpos.z = min(pos.z, Minpos.z);
				//���̒��_�ցB
				pData += mesh->vertexStride;
			}
			//���_�o�b�t�@���A�����b�N
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
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
	m_box.origin = origin;
	m_box.direction[0] = CVector3::AxisX();
	m_box.direction[1] = CVector3::AxisY();
	m_box.direction[2] = CVector3::AxisZ();
	m_box.directionLen.x = directionXLen;	
	m_box.directionLen.y = directionYLen;
	m_box.directionLen.z = directionZLen;



	CameraAngle = g_camera3D.GetViewAngle();
	m_enFbxUpAxis = enFbxUpAxis;
	//if (maxInstance <= 1) {
	smLightManager().AddSkinModel(this);
	//}
}

bool SkinModel::init(const wchar_t* filePath)
{
	m_modelDx = g_skinModelDataManager.Load(filePath);
	if (m_modelDx != NULL) {
		return true;
	}
	return false;
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
	result = m_skeleton.Load(skeletonFilePath.c_str());
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
void SkinModel::InitSamplerState(int maxInstance)
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
		//�C���X�^���V���O�p�̃f�[�^���쐬�B
		if (maxInstance > 1) {
			m_instancingData[i].resize(maxInstance);
			m_Matrix[i].resize(maxInstance);
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRV�Ƃ��ăo�C���h�\�B
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
		m_instancingScale[No].push_back(m_scale);
		if (result) {
			//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
			CMatrix mBias = CMatrix::Identity();
			if (m_enFbxUpAxis == enFbxUpAxisZ) {
				//Z-upttt
				mBias.MakeRotationX(CMath::PI * -0.5f);
			}
			//�X�L������p�̍s��𐶐�
			CMatrix transMatrix, rotMatrix, scaleMatrix;
			//���s�ړ��s����쐬����B
			transMatrix.MakeTranslation(trans);
			//��]�s����쐬����B
			rotMatrix = CMatrix::Identity();// MakeRotationFromQuaternion(rot);
			//�g��s����쐬����B
			scaleMatrix = CMatrix::Identity();
			//���[���h�s����쐬����B
			//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
			//���Ԃ��ԈႦ���猋�ʂ��ς���B
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
	m_scale = scale;
	scaleMatrix.MakeScaling(m_scale);
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

}
void SkinModel::Draw(EnDrawMode drawMode, CMatrix viewMatrix, CMatrix projMatrix)
{

	auto No = GetSkinModelManager().GetNo();
	if (drawMode == enShadow|| drawMode == enCubeMap) {
		m_vsSCb[No].mWorld = m_worldMatrix;
		m_vsSCb[No].mProj = projMatrix;
		m_vsSCb[No].mView = viewMatrix;
		auto delayNo = No;
		delayNo++;
		delayNo %= 2;
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
		if (m_numInstance > 1) {
			//�C���X�^���V���O�p�̃f�[�^���X�V�B
			if (result)
			{
				m_Matrix[delayNo][m_maxInstance - 1] = CMatrix::Identity();
				for (int i = 0; i < m_maxInstance; i++)
				{
					m_instancingData[delayNo][i] = m_Matrix[delayNo][i];
				}
			}	
			if (m_drawData[0].size() > 0) {
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRV�Ƃ��ăo�C���h�\�B
				desc.ByteWidth = sizeof(CMatrix) * m_drawData[0].size();
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = sizeof(CMatrix);
				m_instancingDataSB[delayNo].Create(m_drawData[0].data(), desc);
				d3dDeviceContext->UpdateSubresource(m_instancingDataSB[delayNo].GetBody(), 0, NULL, m_drawData[0].data(), 0, 0);
				d3dDeviceContext->VSSetShaderResources(100, 1, &(m_instancingDataSB[delayNo].GetSRV()).GetBody());
			}
			else
			{
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRV�Ƃ��ăo�C���h�\�B
				desc.ByteWidth = sizeof(CMatrix) * m_numInstance;
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = sizeof(CMatrix);
				m_instancingDataSB[delayNo].Create(m_instancingData[delayNo].data(), desc);		
				d3dDeviceContext->UpdateSubresource(m_instancingDataSB[delayNo].GetBody(), 0, NULL, m_instancingData[delayNo].data(), 0, 0);
				d3dDeviceContext->VSSetShaderResources(100, 1, &(m_instancingDataSB[delayNo].GetSRV()).GetBody());
			}
		}
		//�萔�o�b�t�@�̓��e���X�V�B
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
		//�萔�o�b�t�@��GPU�ɓ]���B
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
		//�T���v���X�e�[�g��ݒ�B
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[delayNo]);
		//�{�[���s���GPU�ɓ]���B
		m_skeleton.Update(m_vsSCb[delayNo].mWorld);
		m_skeleton.SendBoneMatrixArrayToGPU();

		FindMesh([&](auto& ef) {
			ModelEffect* effect = (ModelEffect*)ef->effect.get();
			effect->SetDrawMode(drawMode);
			if (m_numInstance > 1)
			{
				effect->SetDrawMode(enInstancingShadow);
			}
			if (m_Mode[delayNo] == enTree)
			{
				effect->SetDrawMode(enTreeShadow);
			}
		});
		//�`��B
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_vsSCb[delayNo].mWorld,
			m_vsSCb[No].mView,
			m_vsSCb[No].mProj,
			false,
			nullptr,
			m_drawData[0].size() > 1 ? m_drawData[0].size() : 1
		);
		if (m_weapon)
		{
			for (auto& modelMeshs : m_modelDxweapon->meshes) {
				for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
					ModelEffect* effect = (ModelEffect*)mesh->effect.get();
					effect->SetDrawMode(drawMode);
					if (m_numInstance > 1)
					{
						effect->SetDrawMode(enInstancingShadow);
					}
					if (m_Mode[delayNo] == enTree)
					{
						effect->SetDrawMode(enTreeShadow);
					}
				}
			}
			m_modelDxweapon->Draw(
				d3dDeviceContext,
				state,
				m_vsSCb[delayNo].mWorld,
				m_vsSCb[No].mView,
				m_vsSCb[No].mProj,
				false,
				nullptr,
				m_drawData[0].size() > 1 ? m_drawData[0].size() : 1
			);
		}
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
		//�C���X�^���V���O�p�̃f�[�^���X�V�B
		if (result)
		{
			m_Matrix[No][m_maxInstance - 1] = CMatrix::Identity();
			for (int i = 0; i < m_maxInstance; i++)
			{
				m_instancingData[No][i] = m_Matrix[No][i];
			}
		}
		if (m_drawData[1].size() > 0) {
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRV�Ƃ��ăo�C���h�\�B
			desc.ByteWidth = sizeof(CMatrix) * m_drawData[1].size();
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.StructureByteStride = sizeof(CMatrix);
			m_instancingDataSB[No].Create(m_drawData[1].data(), desc);
			d3dDeviceContext->UpdateSubresource(m_instancingDataSB[No].GetBody(), 0, NULL, m_drawData[1].data(), 0, 0);
			d3dDeviceContext->VSSetShaderResources(100, 1, &(m_instancingDataSB[No].GetSRV()).GetBody());
		}
		else
		{
			d3dDeviceContext->UpdateSubresource(m_instancingDataSB[No].GetBody(), 0, NULL, m_instancingData[No].data(), 0, 0);
			d3dDeviceContext->VSSetShaderResources(100, 1, &(m_instancingDataSB[No].GetSRV()).GetBody());
		}
	}
		
	SVSConstantBuffer vsCb;
	//�萔�o�b�t�@�̓��e���X�V�B
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
	//�萔�o�b�t�@��GPU�ɓ]���B
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//�T���v���X�e�[�g��ݒ�B
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState[No]);
	//�{�[���s���GPU�ɓ]���B
	//�X�P���g���̍X�V�B
	m_skeleton.Update(m_vsCb[No].mWorld);
	m_skeleton.SendBoneMatrixArrayToGPU();

	FindMesh([&](auto& ef) {
		ModelEffect* effect = (ModelEffect*)ef->effect.get();
		effect->SetDrawMode(m_Mode[No]);
	});
	auto num = m_drawData[1].size();
	//�`��B
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_vsCb[No].mWorld,
		m_vsCb[No].mView,
		m_vsCb[No].mProj,
		false,
		nullptr,
		num > 1 ? num : 1
	);
	if (m_weapon)
	{
		for (auto& modelMeshs : m_modelDxweapon->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				ModelEffect* effect = (ModelEffect*)mesh->effect.get();
				effect->SetDrawMode(m_Mode[No]);
			}
		}
		m_modelDxweapon->Draw(
			d3dDeviceContext,
			state,
			m_vsCb[No].mWorld,
			m_vsCb[No].mView,
			m_vsCb[No].mProj,
			false,
			nullptr,
			num > 1 ? num : 1
		);
	}
}
void SkinModel::BeginUpdateInstancingData(){
	m_numInstance = 0;
	auto No = GetSkinModelManager().GetNo();
	m_instancingData[No].clear();
	m_instancingScale[No].clear();
}
void SkinModel::CullingInstancing(EnDrawMode drawMode, int No,const Plane m_kaku[6])
{
	int hoge = 0;
	if (drawMode != enShadow)
	{
		hoge = 1;
	}
	m_drawData[hoge].clear();
	auto data = m_instancingData[No].begin();
	int countNo = 0;
	while(data != m_instancingData[No].end()) {
		CMatrix mWorld = (*data);
		CMatrix transMatrix;
		//���s�ړ��s����쐬����B
		transMatrix.MakeTranslation(m_box.origin);
		transMatrix.Mul(mWorld, transMatrix);
		m_box.direction[0].x = transMatrix.m[0][0];//�E
		m_box.direction[0].y = transMatrix.m[0][1];
		m_box.direction[0].z = transMatrix.m[0][2];
		m_box.direction[1].x = transMatrix.m[1][0];//����
		m_box.direction[1].y = transMatrix.m[1][1];
		m_box.direction[1].z = transMatrix.m[1][2];
		m_box.direction[2].x = transMatrix.m[2][0];//�܂�
		m_box.direction[2].y = transMatrix.m[2][1];
		m_box.direction[2].z = transMatrix.m[2][2];
		m_box.direction[0].Normalize();
		m_box.direction[1].Normalize();
		m_box.direction[2].Normalize();

		CQuaternion na;
		na.SetRotationDeg(CVector3::AxisX(), 90.0f);
		na.Multiply(m_box.direction[0]);
		na.Multiply(m_box.direction[1]);
		na.Multiply(m_box.direction[2]);
		m_box.direction[0].Normalize();
		m_box.direction[1].Normalize();
		m_box.direction[2].Normalize();
		CVector3 ni;
		ni.x = transMatrix.m[3][0];//pos
		ni.y = transMatrix.m[3][1];
		ni.z = transMatrix.m[3][2];
		CVector3 Minposa;
		Minposa = ni;
		Minposa -= m_box.direction[0] * (m_box.directionLen.x * m_instancingScale[No][countNo].x);
		Minposa -= m_box.direction[1] * (m_box.directionLen.y * m_instancingScale[No][countNo].y);
		Minposa -= m_box.direction[2] * (m_box.directionLen.z * m_instancingScale[No][countNo].z);
		//CalculateFrustumPlanes(m_vsCb[No].mProj, No);
		m_cameralen = (ni - m_kaku[0].m_centerPos).Length();
		int count = 0;
		for (count = 0; count < 6; count++) {
			if (GetPositivePoint(count, Minposa, m_kaku,No, countNo))
			{
				break;

			}
		}
		//if (count >= 5) {
			m_drawData[hoge].push_back(mWorld);
		//}
		data++;						//����ȊO�͎��ցB
		countNo++;
	}
	//for (int i = 0; i < Notdraw; i++)
	//{
	//	CMatrix kari;
	//	kari.MakeTranslation({100000.0f,100000.0f,100000.0f});
	//	m_drawData[hoge].push_back(kari);
	//}
}
bool SkinModel::Culling(EnDrawMode drawMode, int No,const Plane m_kaku[6])
{
	if (m_instancingData[No].size() < 1){
		auto mWorld = m_vsCb[No].mWorld;
		CMatrix transMatrix;
		//���s�ړ��s����쐬����B
		transMatrix.MakeTranslation(m_box.origin);
		transMatrix.Mul(mWorld, transMatrix);
		m_box.direction[0].x = transMatrix.m[0][0];//�E
		m_box.direction[0].y = transMatrix.m[0][1];
		m_box.direction[0].z = transMatrix.m[0][2];
		m_box.direction[1].x = transMatrix.m[1][0];//����
		m_box.direction[1].y = transMatrix.m[1][1];
		m_box.direction[1].z = transMatrix.m[1][2];
		m_box.direction[2].x = transMatrix.m[2][0];//�܂�
		m_box.direction[2].y = transMatrix.m[2][1];
		m_box.direction[2].z = transMatrix.m[2][2];
		m_box.direction[0].Normalize();
		m_box.direction[1].Normalize();
		m_box.direction[2].Normalize();

		CQuaternion alignment;
		alignment.SetRotationDeg(CVector3::AxisX(), 90.0f);
		m_box.direction[0];
		alignment.Multiply(m_box.direction[0]);
		alignment.Multiply(m_box.direction[1]);
		alignment.Multiply(m_box.direction[2]);
		m_box.direction[0].Normalize();
		m_box.direction[1].Normalize();
		m_box.direction[2].Normalize();
		CVector3 ni;
		ni.x = transMatrix.m[3][0];//�܂�
		ni.y = transMatrix.m[3][1];
		ni.z = transMatrix.m[3][2];
		CVector3 Minposa;
		Minposa = ni;
		Minposa -= m_box.direction[0] * m_box.directionLen.x * m_scale.x;
		Minposa -= m_box.direction[1] * m_box.directionLen.y * m_scale.y;
		Minposa -= m_box.direction[2] * m_box.directionLen.z * m_scale.z;
		//CalculateFrustumPlanes(m_vsCb[No].mProj, No);
		m_cameralen = (ni - m_kaku[0].m_centerPos).Length();
		for (int i = 0; i < 6; i++) {
			if(GetPositivePoint(i, Minposa, m_kaku)) {
					return false;
			}
		}
		return true;
	}
	else
	{
		int hoge = 0;
		if (drawMode != enShadow)
		{
			hoge = 1;
		}
		if (m_instancingData[No].size() > 0)
		{
			CullingInstancing(drawMode,No, m_kaku);

			if (m_drawData[hoge].size() > 0)
			{
				return true;
			}
		}
		return false;
	}
}
/// <summary>
/// �@�������ԋ߂��_���Z�o����
/// </summary>
/// <param name="target">�^�[�Q�b�g�ƂȂ�AABB</param>
/// <param name="normal">�Z�o����@��</param>
/// <returns></returns>
bool SkinModel::GetPositivePoint(int No, CVector3 pos,const Plane m_kaku[6])
{
	float len = FLT_MAX;
	CVector3 bb = pos;
	CVector3 bb211 = CVector3::Zero();
	for (int i = 0; i < 7; i++)
	{
		auto na = m_kaku[No].m_normal.Dot(bb - m_kaku[No].m_centerPos); 
		if (0 < na)
		{
			return false;
		}
		if (i % 2 == 0) {
			//X�̓���
			float fugou = 1.0f;
			if (i / 2 % 2 !=0)
			{
				fugou *= -1.0f;
			}
			bb += m_box.direction[0] * m_box.directionLen.x*( 2.0f*fugou)* m_scale.x;
		}
		if ((i + 3) % 4 == 0)
		{
			float fugou = 1.0f;
			if ((i+3) / 4 % 2 == 0)
			{
				fugou *= -1.0f;
			}
			bb += m_box.direction[1] * m_box.directionLen.y * (2.0f*fugou)* m_scale.y;
		}
		if (i == 3)
		{
			bb += m_box.direction[2] * m_box.directionLen.z * 2.0f* m_scale.z;

		}
	}
	return true;
}
/// �@�������ԋ߂��_���Z�o����
/// </summary>
/// <param name="target">�^�[�Q�b�g�ƂȂ�AABB</param>
/// <param name="normal">�Z�o����@��</param>
/// <returns></returns>
bool SkinModel::GetPositivePoint(int No, CVector3 pos, const Plane m_kaku[6],int nn,int sNo)
{
	float len = FLT_MAX;
	CVector3 bb = pos;
	CVector3 bb211 = CVector3::Zero();
	for (int i = 0; i < 7; i++)
	{
		auto na = m_kaku[No].m_normal.Dot(bb - m_kaku[No].m_centerPos);
		if (0 < na)
		{
			return false;
		}
		if (i % 2 == 0) {
			//X�̓���
			float fugou = 1.0f;
			if (i / 2 % 2 != 0)
			{
				fugou *= -1.0f;
			}
			bb += m_box.direction[0] * m_box.directionLen.x*(2.0f*fugou)* m_instancingScale[nn][sNo].x;
		}
		if ((i + 3) % 4 == 0)
		{
			float fugou = 1.0f;
			if ((i + 3) / 4 % 2 == 0)
			{
				fugou *= -1.0f;
			}
			bb += m_box.direction[1] * m_box.directionLen.y * (2.0f*fugou)* m_instancingScale[nn][sNo].y;
		}
		if (i == 3)
		{
			bb += m_box.direction[2] * m_box.directionLen.z * 2.0f* m_instancingScale[nn][sNo].z;

		}
	}
	return true;
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
//		pos += m_box.direction[0] * m_box.directionLen.x;
//	}
//	if (normal.y < 0)
//	{
//		pos += m_box.direction[1] * m_box.directionLen.y;
//	}
//	if (normal.z < 0)
//	{
//		pos += m_box.direction[2] * m_box.directionLen.z;
//	}
//
//	return pos;
//}
/// <summary>
/// �w�肳�ꂽProjection Matrics���王�����6�ʂ̕��ʂ����߂�
/// </summary>
/// <param name="pmat">Projection Matrix</param>
/// <returns></returns>

