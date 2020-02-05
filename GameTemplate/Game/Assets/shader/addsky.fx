Texture2D<float4> MainColorTexture : register(t0);
Texture2D<float4> skyColorTexture : register(t1);
Texture2D<float4> silhouetteTexture : register(t2);

sampler Sampler : register(s0);

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

float4 PSMain(PSInput In) : SV_Target0
{
	float4 mainColor = MainColorTexture.Sample(Sampler, In.uv);
	float4 skyColor = skyColorTexture.Sample(Sampler, In.uv);
	float silhouette = silhouetteTexture.Sample(Sampler, In.uv).x;
	float4 color = mainColor * silhouette;
	color += skyColor * ((silhouette*-1.0f) + 1.0f);

	return color;
}