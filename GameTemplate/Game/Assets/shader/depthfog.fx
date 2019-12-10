
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
sampler Sampler : register(s0);
Texture2D<float4> srcTexture : register(t0);	//カラーテクスチャ。
Texture2D<float4> depthTexture : register(t1);
cbuffer DepthFogParam:register(b2)
{
	float depthPow;
}

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSCopyMain(PSInput In) : SV_Target0
{
	float4 color = srcTexture.Sample(Sampler, In.uv);
	return color;
}

float4 PSMain(PSInput In) : SV_Target0
{
	float4 color = srcTexture.Sample(Sampler, In.uv);
	float depth = depthTexture.Sample(Sampler, In.uv).x;
	float colorlen = length(color.xyz);
	depth = max(0.0f,pow(depth, depthPow));
	//color = color + float4(depth, depth, depth, 1.0f);
	color = color * (1.0f - depth) + float4(depth, depth, depth, 1.0f);
	color.w = 1.0f;
	return color;
}