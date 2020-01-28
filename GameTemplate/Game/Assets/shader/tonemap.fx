
Texture2D<float4> srcTexture : register(t0);
Texture2D<float4> AveLumiTexture : register(t1);
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
sampler Sampler : register(s0);
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSLuminanceMain(PSInput In) : SV_Target0
{
	float2 texSize;
	float level;
	srcTexture.GetDimensions(0, texSize.x, texSize.y, level);
	float pixSizeX = 1.0f / texSize.x;
	float pixSizeY = 1.0f / texSize.y;
	float w = pixSizeX * (texSize.x / 12.0f);
	float h = pixSizeY * (texSize.y / 12.0f);

	float2 uv = float2(0.5f, 0.5f);
	float4 color = srcTexture.Sample(Sampler, float2(uv.x - w , uv.y));
	color += srcTexture.Sample(Sampler, float2(uv.x + w , uv.y));
	color += srcTexture.Sample(Sampler, float2(uv.x, uv.y - h));
	color += srcTexture.Sample(Sampler, float2(uv.x, uv.y + h));

	color += srcTexture.Sample(Sampler, float2(uv.x - w * 2.0f , uv.y));
	color += srcTexture.Sample(Sampler, float2(uv.x + w * 2.0f , uv.y));
	color += srcTexture.Sample(Sampler, float2(uv.x, uv.y - h * 2.0f));
	color += srcTexture.Sample(Sampler, float2(uv.x, uv.y + h * 2.0f));

	color += srcTexture.Sample(Sampler, float2(uv.x - w, uv.y - h));
	color += srcTexture.Sample(Sampler, float2(uv.x + w, uv.y - h));
	color += srcTexture.Sample(Sampler, float2(uv.x - w, uv.y + h));
	color += srcTexture.Sample(Sampler, float2(uv.x + w, uv.y + h));

	color += srcTexture.Sample(Sampler, float2(uv.x - w * 2.0f, uv.y - h * 2.0f));
	color += srcTexture.Sample(Sampler, float2(uv.x + w * 2.0f, uv.y - h * 2.0f));
	color += srcTexture.Sample(Sampler, float2(uv.x - w * 2.0f, uv.y + h * 2.0f));
	color += srcTexture.Sample(Sampler, float2(uv.x + w * 2.0f, uv.y + h * 2.0f));

	color /= 16.0f;
	color.w = 1.0f;


	return color;
}

float4 PSCopyMain(PSInput In) : SV_Target0
{
	float4 color = srcTexture.Sample(Sampler, In.uv);
	return color;
}

float4 PSFinalMain(PSInput In) : SV_Target0
{
	float4 luminance = AveLumiTexture.Sample(Sampler, float2(0.0f,0.0f));
	float4 srcColor = srcTexture.Sample(Sampler, In.uv);
	float ave = 1.0f / max(0.0001f,dot(luminance.xyz, float3(0.2125f, 0.7154f, 0.0721f)));
	//float ave = 0.33333f / max(0.000001f, t);
	float4 color = srcColor * ave*0.33333f;
	color.w = 1.0f;
	return color;
}