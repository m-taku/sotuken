
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
sampler Sampler : register(s0);
Texture2D<float4> colorTexture : register(t0);	//カラーテクスチャ。

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
	float4 color = colorTexture.Sample(Sampler, In.uv);
	color.x *= 4.0f;
	color.w = 1.0f;
	return color;
}