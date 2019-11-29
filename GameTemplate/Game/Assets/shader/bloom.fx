Texture2D<float4> SrcTexture :	register(t0);
Texture2D<float4> FinalBlurTexture :	register(t0);
Texture2D<float4> Blur1Texture :	register(t0);
Texture2D<float4> Blur2Texture :	register(t1);
Texture2D<float4> Blur3Texture :	register(t2);
Texture2D<float4> Blur4Texture :	register(t3);
sampler Sampler : register(s0);

struct VSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

struct PSInput {
	float4 pos:SV_POSITION;
	float2 uv : TEXCOORD0;
};

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSLuminance(PSInput In) :SV_Target0
{
	
	float4 color = SrcTexture.Sample(Sampler, In.uv);
	float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	clip(t - 1.001f);			//輝度が1.0以下ならピクセルキル
	color.xyz *= (t - 1.0f);
	color.a = 1.0f;
	return color;
}

float4 PSMixBlurTex(PSInput In) : SV_Target0
{

	float4 color = Blur1Texture.Sample(Sampler, In.uv);
	color += Blur2Texture.Sample(Sampler, In.uv);
	color += Blur3Texture.Sample(Sampler, In.uv);
	color += Blur4Texture.Sample(Sampler, In.uv);
	color /= 4.0f;
	color.w = 1.0f;
	return color;
}

float4 PSFinal(PSInput In) : SV_Target0
{
	float4 color = FinalBlurTexture.Sample(Sampler, In.uv);
	return color;
}