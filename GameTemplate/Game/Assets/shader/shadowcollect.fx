Texture2D<float4> ShadowMap : register(t0);

Texture2D<float4> WorldTexture : register(t1);

Texture2D<float4> PreCollectTexture : register(t0);

Texture2D<float4> FinalCollectTexture : register(t1);
Texture2D<float4> CopyTexture : register(t0);
Texture2D<float4> ShadowCollectTexture : register(t0);
Texture2D<float4> MainColorTexture : register(t1);




/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);
/////////////////////////////////////////////////////////////

cbuffer LVP:register(b0) {
	float4x4 LightViewMatrix;
	float4x4 LightProjectionMatrix;
}

struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSInitMain(PSInput In) : SV_Target0
{
	return float4(1.0f,1.0f,1.0f,1.0f);
}

float4 PSMain(PSInput In) : SV_Target0
{
	float4 World = float4(WorldTexture.Sample(Sampler, In.uv).xyz,1.0f);
	float4 LUV;
	LUV = mul(LightViewMatrix, World);
	LUV = mul(LightProjectionMatrix, LUV);
	LUV.xyz /= LUV.w;
	float Z = min(LUV.z, 1.0f);
	LUV.xy *= float2(0.5f, -0.5f);
	LUV.xy += 0.5f;
	float4 s = 1.0f;
	if (
		LUV.x < 1.0f
		&& LUV.x > 0.0f
		&& LUV.y < 1.0f
		&& LUV.y > 0.0f
		)
	{
		float mapZ = ShadowMap.Sample(Sampler, LUV.xy).x;
		if (Z > mapZ + 0.0005f)
		{
			s.xyz *= 0.0f;
		}
	}
	
	return s;
}

float4 PSCollectMain(PSInput In) : SV_Target0
{
	float4 c = PreCollectTexture.Sample(Sampler, In.uv);
	float4 color = FinalCollectTexture.Sample(Sampler, In.uv);
	color *= c;
	return color;
}

float4 PSCopyMain(PSInput In) : SV_Target0
{
	float4 color =CopyTexture.Sample(Sampler, In.uv);
	return color;
}

float4 PSFinalMain(PSInput In) : SV_Target0
{
	float4 color = MainColorTexture.Sample(Sampler, In.uv);
	float4 shadowColor = ShadowCollectTexture.Sample(Sampler, In.uv);
	color *= shadowColor;
	color.w = 1.0f;
	return color;
}