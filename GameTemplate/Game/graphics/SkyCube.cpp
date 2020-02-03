#include "stdafx.h"
#include "SkyCube.h"
#include "graphics/SkinModelEffect.h"


SkyCube::SkyCube()
{
}


SkyCube::~SkyCube()
{
}

void SkyCube::Init(const wchar_t* modelFilePath, const wchar_t* cubeMapFilePath)
{
	m_skinModel.Init(modelFilePath);
	m_cubeMapSRV.CreateFromDDSTextureFromFile(cubeMapFilePath);
	m_skinModel.FindMesh([&](auto& effect)
	{
		auto* modelEffect = (ModelEffect*)effect->effect.get();
		modelEffect->SetAlbedoTexture(m_cubeMapSRV.GetBody(), false);
	});
}

void SkyCube::Update(const CVector3& position, const CVector3& scale)
{
	m_skinModel.UpdateWorldMatrix(position, CQuaternion::Identity(), scale);
}

void SkyCube::Draw()
{
	ID3D11DeviceContext* devicecontext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11ShaderResourceView* srv[] = {
		g_graphicsEngine->GetDeferredRender().GetDepthSRV()
	};
	devicecontext->PSGetShaderResources(1, 1, srv);
	m_skinModel.Draw(
		enCubeMap,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
