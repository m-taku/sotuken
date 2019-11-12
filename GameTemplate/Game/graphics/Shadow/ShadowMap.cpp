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
	m_cb.Create(NULL, sizeof(SShadowCollectCB));
}

void ShadowMap::UpdateDirection(const CVector3 & Direction)
{
	float LightCamHight = g_graphicsEngine->GetShadowMapHight();
	CVector3 LigCamTarget[enMapNum];
	CVector3 GameCamPos = smGameCamera().GetCameraPosition();
	CVector3 GameCamTarget = smGameCamera().GetCameraTarget();
	CVector3 GameCamXZ = smGameCamera().GetCameraFoward();
	GameCamXZ.y = 0.0f;
	GameCamXZ.Normalize();
	CVector3 CameraRight = smGameCamera().GetCameraRight();
	CVector3 ToGameCamTarget = smGameCamera().GetCameraTarget() - smGameCamera().GetCameraPosition();
	CVector3 direction = Direction;
	direction.Normalize();
	float AvailableLen = g_graphicsEngine->GetShadowAvailableLength();
	float ViewAngle = g_camera3D.GetViewAngle();
	float scaleX = 1.0f - fabsf(CameraRight.Dot(direction));
	float scaleY = 1.0f - fabsf(GameCamXZ.Dot(direction));

	for (int i = 0; i < enMapNum; i++)
	{
		switch (i)
		{
		case enShadowMap: {
			m_shadowHight[i] = AvailableLen * m_cascadePerHigh;
			break;
		}
		case enCascadeNear: {
			m_shadowHight[i] = AvailableLen * m_cascadePerMidle;
			break;
		}
		case enCascadeFar: {
			m_shadowHight[i] = AvailableLen * m_cascadePerLow;
			break;
		}
		default:
			break;
		}
		if (i > 0)
		{
			LigCamTarget[i] = LigCamTarget[i - 1] + (GameCamXZ*((m_shadowHight[i - 1] * 0.5f) + (m_shadowHight[i] * 0.5f)));
			float len = CVector3(LigCamTarget[i] - GameCamPos).Length() + (m_shadowHight[i] * 0.5f);
			m_shadowWidth[i] = (tanf(ViewAngle)*len)*2.0f;
		}
		else
		{
			LigCamTarget[i] = GameCamTarget;
			float len = CVector3(LigCamTarget[i] - GameCamPos).Length() + (m_shadowHight[i] * 0.5f);
			m_shadowWidth[i] = (tanf(ViewAngle)*len)*2.0f;

		}
		LigCamTarget[i] += CameraRight * (m_shadowWidth[i] * 0.5f) * CameraRight.Dot(direction);
		LigCamTarget[i] += GameCamXZ * (m_shadowHight[i] * 0.5f) * GameCamXZ.Dot(direction);
		/*m_shadowWidth[i] *= scaleX;
		m_shadowHight[i] *= scaleY;*/

		m_lightViewMatrix[i].MakeLookAt(
			LigCamTarget[i] + direction * -LightCamHight,
			LigCamTarget[i],
			GameCamXZ
		);

		m_lightProjectionMatrix[i].MakeOrthoProjectionMatrix(
			m_shadowWidth[i],
			m_shadowHight[i],
			1.0f,
			LightCamHight
		);
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



	for (int i = 0; i < enMapNum; i++)
	{
		ID3D11RenderTargetView* rtv[] = {
			m_renderTarget[i].GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, rtv, m_renderTarget[i].GetDepthStencilView());
		deviceContext->RSSetViewports(1, m_renderTarget[i].GetViewPort());
		float color[] = { 1.0f,1.0f,1.0f,1.0f };
		m_renderTarget[i].Clear(color);
		for (const auto& shadowcaster : smLightManager().GetSkinModel())
		{
			if (shadowcaster->IsShadowCaster())
			{
				shadowcaster->Draw(
					enShadow,
					m_lightViewMatrix[i],
					m_lightProjectionMatrix[i]
				);
			}
		}
	}



	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
	float color[] = { 1.0f,1.0f,1.0f,1.0f };
	deviceContext->ClearRenderTargetView(buckUpRTV, color);
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
		m_postEffect.SetPS("Assets/shader/shadowcollect.fx", "PSMain");
		m_postEffect.SetVS("Assets/shader/shadowcollect.fx", "VSMain");

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
			g_graphicsEngine->GetDeferredRender().GetWorldSRV(),
		};

		deviceContext->VSSetShaderResources(0, 2, precollectsrv);
		deviceContext->PSSetShaderResources(0, 2, precollectsrv);
		m_postEffect.Draw();

		ID3D11RenderTargetView* collectrtv[] = {
			FinalCollectRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, collectrtv, FinalCollectRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, FinalCollectRT.GetViewPort());
		deviceContext->ClearDepthStencilView(smLightManager().GetShadowRenderTarget().GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_postEffect.SetPS("Assets/shader/shadowcollect.fx", "PSCollectMain");

		ID3D11ShaderResourceView* collectsrv[] = {
			preCollectRT.GetShaderResourceView(),
			smLightManager().GetShadowRenderTarget().GetShaderResourceView()
		};

		deviceContext->VSSetShaderResources(0, 2, collectsrv);
		deviceContext->PSSetShaderResources(0, 2, collectsrv);
		m_postEffect.Draw();

		ID3D11RenderTargetView* copyrtv[] = {
			smLightManager().GetShadowRenderTarget().GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, copyrtv, smLightManager().GetShadowRenderTarget().GetDepthStencilView());
		deviceContext->RSSetViewports(1, smLightManager().GetShadowRenderTarget().GetViewPort());
		deviceContext->ClearDepthStencilView(smLightManager().GetShadowRenderTarget().GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_postEffect.SetPS("Assets/shader/shadowcollect.fx", "PSCopyMain");

		ID3D11ShaderResourceView* copysrv[] = {
			FinalCollectRT.GetShaderResourceView()
		};

		deviceContext->VSSetShaderResources(0, 1, copysrv);
		deviceContext->PSSetShaderResources(0, 1, copysrv);
		m_postEffect.Draw();
	}

	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
}
