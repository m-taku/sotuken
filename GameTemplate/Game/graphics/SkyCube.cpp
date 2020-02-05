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
	m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_addRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_fullScreenDraw.Init();
	m_copyVS.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);
	m_copyPS.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);


	m_addVS.Load("Assets/shader/addsky.fx", "VSMain", Shader::EnType::VS);
	m_addPS.Load("Assets/shader/addsky.fx", "PSMain", Shader::EnType::PS);


	m_skinModel.Init(modelFilePath);
	m_cubeMapSRV.CreateFromDDSTextureFromFile(cubeMapFilePath);
	m_skinModel.FindMesh([&](auto& effect)
	{
		auto* modelEffect = (ModelEffect*)effect->effect.get();
		modelEffect->SetAlbedoTexture(m_cubeMapSRV.GetBody(),true);
	});
}

void SkyCube::Update(const CVector3& position, const CVector3& scale)
{
	m_skinModel.UpdateWorldMatrix(position, CQuaternion::Identity(), scale);
	CMatrix view;
	CMatrix proj;
	view.MakeLookAt(
		g_camera3D.GetPosition(),
		g_camera3D.GetTarget(),
		g_camera3D.GetUp()
	);
	proj.MakeProjectionMatrix(
		g_camera3D.GetViewAngle(),
		FRAME_BUFFER_W / FRAME_BUFFER_H,
		g_camera3D.GetNear(),
		scale.x*2.0f
	);
	ViewMatrix = view;
	ProjectionMatrix = proj;
}

void SkyCube::PostDraw()
{

	ID3D11DeviceContext* devicecontext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11RenderTargetView* buckUpRTV = nullptr;
	ID3D11DepthStencilView* buckUpDepth = nullptr;
	CD3D11_VIEWPORT buckUpViewPort;
	UINT numViewPorts = 1;
	devicecontext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDepth);
	devicecontext->RSGetViewports(&numViewPorts, &buckUpViewPort);
	float color[] = { 0.0f,0.0f,0.0f,1.0f };
	m_renderTarget.Clear(color);
	ID3D11RenderTargetView* cubertv = {
		m_renderTarget.GetRenderTatgetView()
	};
	devicecontext->OMSetRenderTargets(1, &cubertv, m_renderTarget.GetDepthStencilView());
	devicecontext->RSSetViewports(1, m_renderTarget.GetViewPort());
	m_skinModel.Draw(
		enCubeMap,
		ViewMatrix,
		ProjectionMatrix
	);
	m_addRenderTarget.Clear(color);
	ID3D11RenderTargetView* addrtv = {
		m_addRenderTarget.GetRenderTatgetView()
	};
	devicecontext->OMSetRenderTargets(1, &addrtv, m_addRenderTarget.GetDepthStencilView());
	devicecontext->RSSetViewports(1, m_addRenderTarget.GetViewPort());

	ID3D11ShaderResourceView* addSRV[] = {
		g_graphicsEngine->GetMainRenderTarget().GetShaderResourceView(),
		m_renderTarget.GetShaderResourceView(),
		g_graphicsEngine->GetDeferredRender().GetSilhouetteSRV()
	};
	devicecontext->PSSetShaderResources(0, 3, addSRV);
	m_fullScreenDraw.SetVS(&m_addVS);
	m_fullScreenDraw.SetPS(&m_addPS);
	m_fullScreenDraw.Draw();

	devicecontext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	devicecontext->RSSetViewports(numViewPorts, &buckUpViewPort);

	ID3D11ShaderResourceView* copySRV[] = {
		m_addRenderTarget.GetShaderResourceView()
	};

	devicecontext->PSSetShaderResources(0, 1, copySRV);
	m_fullScreenDraw.SetVS(&m_copyVS);
	m_fullScreenDraw.SetPS(&m_copyPS);
	m_fullScreenDraw.Draw();
}
