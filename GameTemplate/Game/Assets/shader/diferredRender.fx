/*!
 * @brief	モデルシェーダー。
 */


 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
 //アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> NormalTexture : register(t1);
Texture2D<float4> WorldTexture : register(t2);
Texture2D<float4> DepthTexture : register(t3);
Texture2D<float4> ShadowTexture : register(t4);

struct SDirectionLight {
	float4 color;
	float3 direction;
};

struct SPointLight {
	float4 color;
	float4 attn;
	float3 position;
};

StructuredBuffer<SDirectionLight>DirectionLightSB:register(t100);
StructuredBuffer<SPointLight>PointLightSB:register(t101);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);
/////////////////////////////////////////////////////////////
cbuffer LightParam:register(b1) {
	float3 eyePos;
	int numDirectionLight;
	int numPointLight;
}

struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

struct NWD {
	float3 Normal;
	float3 World;
	float Depth;
};

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}
////////////////////////////////////////////////////////////////////////////////////////
float4 DirectionLight(NWD nwd)
{
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < numDirectionLight; i++)
	{
		float4 color = DirectionLightSB[i].color;
		float3 direction = DirectionLightSB[i].direction;
		float dotResult = max(0.0f, dot(-direction, nwd.Normal));
		color *= dotResult;
		finalColor += color;
	}
	finalColor.w = 1.0f;
	return finalColor;
}

float4 PointLight(NWD nwd)
{
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < numPointLight; i++)
	{
		float4 color = PointLightSB[i].color;
		float4 attn = PointLightSB[i].attn;
		float3 position = PointLightSB[i].position;
		float3 direction = nwd.World - position;
		float len = length(direction);
		direction = normalize(direction);
		float dotResult = max(0.0f, dot(-direction, nwd.Normal));
		color *= dotResult;
		float t = max(0.0f, attn.x - len) / attn.x;
		color *= pow(t, attn.y);
		finalColor += color;
	}
	finalColor.w = 1.0f;
	return finalColor;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	NWD nwd;
	float4 color = albedoTexture.Sample(Sampler, In.uv);
	float4 buckUpColor = color;
	nwd.Normal = NormalTexture.Sample(Sampler, In.uv).xyz;
	nwd.World = WorldTexture.Sample(Sampler, In.uv).xyz;
	//nwd.Depth = DepthTexture.Sample(Sampler, In.uv).x;
	float4 shadow = ShadowTexture.Sample(Sampler, In.uv);
	color *= max(0.1f,DirectionLight(nwd)*shadow);
	color += buckUpColor * PointLight(nwd);
	color.w = 1.0f;
	return color;
}

