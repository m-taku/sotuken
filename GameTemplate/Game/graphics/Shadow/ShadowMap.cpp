#include "stdafx.h"
#include "ShadowMap.h"


ShadowMap::ShadowMap()
{
}


ShadowMap::~ShadowMap()
{
}

void ShadowMap::Init(int w, int h)
{
	for (int i = 0; i < enMapNum; i++)
	{
		m_renderTarget[i].Create(
			w,
			h,
			DXGI_FORMAT_R32_FLOAT
		);
	}
	m_postEffect.Init();
	m_preCollectPS.Load("Assets/shader/shadowcollect.fx", "PSMain", Shader::EnType::PS);
	m_collectPS.Load("Assets/shader/shadowcollect.fx", "PSCollectMain", Shader::EnType::PS);
	m_copyPS.Load("Assets/shader/shadowcollect.fx", "PSCopyMain", Shader::EnType::PS);
	m_vs.Load("Assets/shader/shadowcollect.fx", "VSMain", Shader::EnType::VS);
	m_postEffect.SetVS(&m_vs);
	m_cb.Create(NULL, sizeof(SShadowCollectCB));
	m_drawCB.Create(NULL, sizeof(SDrawCB));
}

void ShadowMap::UpdateDirection(const CVector3 & Direction)
{
	float AvailableLen = g_graphicsEngine->GetShadowAvailableLength();
	float g_lightHeight = g_graphicsEngine->GetShadowMapHight();
	CVector3 LightDir = Direction;
	LightDir.Normalize();
	CVector3 LightViewForward = LightDir;
	CVector3 LightViewUp;
	if (fabsf(LightViewForward.y) > 0.999f)
	{
		LightViewUp.Cross(LightViewForward, CVector3::Right());
	}
	else
	{
		LightViewUp.Cross(LightViewForward, CVector3::Up());
	}
	LightViewUp.Normalize();
	CVector3 LightViewRight;
	LightViewRight.Cross(LightViewUp, LightViewForward);
	LightViewRight.Normalize();

	CMatrix LightViewRot;

	LightViewRot.m[0][0] = LightViewRight.x;
	LightViewRot.m[0][1] = LightViewRight.y;
	LightViewRot.m[0][2] = LightViewRight.z;
	LightViewRot.m[0][3] = 0.0f;
	LightViewRot.m[1][0] = LightViewUp.x;
	LightViewRot.m[1][1] = LightViewUp.y;
	LightViewRot.m[1][2] = LightViewUp.z;
	LightViewRot.m[1][3] = 0.0f;
	LightViewRot.m[2][0] = LightViewForward.x;
	LightViewRot.m[2][1] = LightViewForward.y;
	LightViewRot.m[2][2] = LightViewForward.z;
	LightViewRot.m[2][3] = 0.0f;

	float ShadowAreaTbl[enMapNum];
	float LightHeight = g_camera3D.GetTarget().y + g_lightHeight;
	CVector3 GameCamPos = g_camera3D.GetPosition();
	CVector3 GameCamForward = g_camera3D.GetForward();
	CVector3 GameCamRight = g_camera3D.GetRight();
	CVector3 GameCamUp;
	GameCamUp.Cross(GameCamForward, GameCamRight);
	GameCamUp.Normalize();
	float NearPlaneZ = 0.0f;
	float FarPlaneZ;
	//float sub = 1.0f / enMapNum;
	float sub[] = {
		0.15f,
		0.3f,
		0.55f
	};
	float aspect = FRAME_BUFFER_W / FRAME_BUFFER_H;
	for (int i = 0; i < enMapNum; i++)
	{
		ShadowAreaTbl[i] = AvailableLen * sub[i];
		FarPlaneZ = NearPlaneZ + ShadowAreaTbl[i];
		CMatrix mLightView = CMatrix::Identity();
		float HalfViewAngle = g_camera3D.GetViewAngle()*0.5f;
		float width, height;
		float farZ = -1.0f;
		CVector3 pos[8];
		{
			float t = tanf(HalfViewAngle);
			CVector3 toUpNear, toUpFar;
			toUpNear = GameCamUp * t*NearPlaneZ;
			toUpFar = GameCamUp * t*FarPlaneZ;
			CVector3 NearPlaneCenterPos = GameCamPos + GameCamForward * NearPlaneZ;
			pos[0] = NearPlaneCenterPos + GameCamRight * t * NearPlaneZ * aspect + toUpNear;
			pos[1] = pos[0] + toUpNear * -2.0f;
			pos[2] = NearPlaneCenterPos + GameCamRight * -t * NearPlaneZ*aspect + toUpNear;
			pos[3] = pos[2] + toUpNear * -2.0f;
			CVector3 FarPlaneCenterPos = GameCamPos + GameCamForward * FarPlaneZ;
			pos[4] = FarPlaneCenterPos + GameCamRight * t*FarPlaneZ*aspect + toUpFar;
			pos[5] = pos[4] + toUpFar * -2.0f;
			pos[6] = FarPlaneCenterPos + GameCamRight * -t * FarPlaneZ*aspect + toUpFar;
			pos[7] = pos[6] + toUpFar * -2.0f;
			CVector3 GameCamFrustumCenter = NearPlaneCenterPos + CVector3(FarPlaneCenterPos - NearPlaneCenterPos) * 0.5f;

			float alpha = (LightHeight - GameCamFrustumCenter.y) / LightDir.y;
			CVector3 LightPos = GameCamFrustumCenter + LightDir * alpha;

			mLightView = LightViewRot;
			mLightView.m[3][0] = LightPos.x;
			mLightView.m[3][1] = LightPos.y;
			mLightView.m[3][2] = LightPos.z;
			mLightView.m[3][3] = 1.0f;
			mLightView.Inverse(mLightView);

			CVector3 posMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			CVector3 posMin = { FLT_MAX, FLT_MAX, FLT_MAX };
			for (auto& InLight : pos) {
				mLightView.Mul(InLight);
				posMax.Max(InLight);
				posMin.Min(InLight);

			}
			width = posMax.x - posMin.x;
			height = posMax.y - posMin.y;
			farZ = posMax.z;
		}
		CMatrix proj;
		proj.MakeOrthoProjectionMatrix(
			width,
			height,
			1.0f,
			farZ
		);
		m_Camera[i].height = height;
		m_Camera[i].width = width;
		m_Camera[i].farZ = farZ;
		NearPlaneZ = FarPlaneZ;
		m_lightViewMatrix[i] = mLightView;
		m_lightProjectionMatrix[i] = proj;
	}
}

void ShadowMap::ShadowCasterDraw()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();

	ID3D11RenderTargetView* buckUpRTV = nullptr;
	ID3D11DepthStencilView* buckUpDepth = nullptr;
	D3D11_VIEWPORT buckUpViewPort;
	UINT numViewPort = 1;
	UINT* pnumViewPort = &numViewPort;
	deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDepth);
	deviceContext->RSGetViewports(pnumViewPort, &buckUpViewPort);

	SDrawCB cb;
	for (int i = 0; i < enMapNum; i++)
	{
		cb.m_lightViewMatrix[i] = m_lightViewMatrix[i];
		cb.m_lightProjectionMatrix[i] = m_lightProjectionMatrix[i];
	}

	for (int i = 0; i < enMapNum; i++)
	{
		cb.No = i;

		ID3D11RenderTargetView* rtv[] = {
			m_renderTarget[i].GetRenderTatgetView()
		};
		deviceContext->UpdateSubresource(m_drawCB.GetBody(), 0, NULL, &cb, 0, 0);
		deviceContext->PSSetConstantBuffers(5, 1, &m_drawCB.GetBody());
		deviceContext->VSSetConstantBuffers(5, 1, &m_drawCB.GetBody());
		deviceContext->OMSetRenderTargets(1, rtv, m_renderTarget[i].GetDepthStencilView());
		deviceContext->RSSetViewports(1, m_renderTarget[i].GetViewPort());
		float color[] = { 1.0f,1.0f,1.0f,1.0f };
		m_renderTarget[i].Clear(color);
		auto n = GetSkinModelManager().SCalculateFrustumPlanes(m_lightViewMatrix[i], m_Camera[i].height, m_Camera[i].width, m_Camera[i].farZ);
		auto No = GetSkinModelManager().GetNo();
		for (const auto& shadowcaster : smLightManager().GetSkinModel())
		{
			if (shadowcaster->IsShadowCaster())
			{

				if (shadowcaster->Culling(enShadow,No, n)) {
					shadowcaster->Draw(
						enShadow,
						m_lightViewMatrix[i],
						m_lightProjectionMatrix[i]
					);
				}
			}
		}
	}

	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
	float color[] = { 1.0f,1.0f,1.0f,1.0f };
	deviceContext->ClearDepthStencilView(buckUpDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMap::DrawToShadowCollector()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11RenderTargetView* buckUpRTV = nullptr;
	ID3D11DepthStencilView* buckUpDepth = nullptr;
	D3D11_VIEWPORT buckUpViewPort;
	UINT numViewPort = 1;
	UINT* pnumViewPort = &numViewPort;
	deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDepth);
	deviceContext->RSGetViewports(pnumViewPort, &buckUpViewPort);


	SShadowCollectCB CB;
	float color[] = { 1.0f,1.0f,1.0f,1.0f };
	RenderTarget preCollectRT;
	preCollectRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	RenderTarget FinalCollectRT;
	FinalCollectRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);


	for (int i = 0; i < enMapNum; i++)
	{
		FinalCollectRT.Clear(color);
		preCollectRT.Clear(color);
		

		ID3D11RenderTargetView* precollectrtv[] = {
			preCollectRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, precollectrtv, preCollectRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, preCollectRT.GetViewPort());

		CB.m_lightViewMatrix = m_lightViewMatrix[i];
		CB.m_lightProjectionMatrix = m_lightProjectionMatrix[i];

		deviceContext->UpdateSubresource(m_cb.GetBody(), 0, NULL, &CB, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_cb.GetBody());
		deviceContext->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		ID3D11ShaderResourceView* precollectsrv[] = {
			m_renderTarget[i].GetShaderResourceView(),
			smLightManager().GetShadowRenderTarget().GetShaderResourceView(),
			g_graphicsEngine->GetDeferredRender().GetWorldSRV()
		};

		deviceContext->VSSetShaderResources(0, 3, precollectsrv);
		deviceContext->PSSetShaderResources(0, 3, precollectsrv);
		m_postEffect.SetPS(&m_preCollectPS);
		m_postEffect.Draw();

		ID3D11RenderTargetView* collectrtv[] = {
			FinalCollectRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, collectrtv, FinalCollectRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, FinalCollectRT.GetViewPort());
		deviceContext->ClearDepthStencilView(smLightManager().GetShadowRenderTarget().GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		

		ID3D11ShaderResourceView* collectsrv[] = {
			preCollectRT.GetShaderResourceView(),
			smLightManager().GetShadowRenderTarget().GetShaderResourceView()
		};

		deviceContext->VSSetShaderResources(0, 2, collectsrv);
		deviceContext->PSSetShaderResources(0, 2, collectsrv);
		m_postEffect.SetPS(&m_collectPS);
		m_postEffect.Draw();

		ID3D11RenderTargetView* copyrtv[] = {
			smLightManager().GetShadowRenderTarget().GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, copyrtv, smLightManager().GetShadowRenderTarget().GetDepthStencilView());
		deviceContext->RSSetViewports(1, smLightManager().GetShadowRenderTarget().GetViewPort());
		deviceContext->ClearDepthStencilView(smLightManager().GetShadowRenderTarget().GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		

		ID3D11ShaderResourceView* copysrv[] = {
			FinalCollectRT.GetShaderResourceView()
		};

		deviceContext->VSSetShaderResources(0, 1, copysrv);
		deviceContext->PSSetShaderResources(0, 1, copysrv);
		m_postEffect.SetPS(&m_copyPS);
		m_postEffect.Draw();
	}

	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
}
