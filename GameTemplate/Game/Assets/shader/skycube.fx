
TextureCube<float4> skyCubeMap : register(t0);	//スカイキューブマップ。
Texture2D<float4> depthTexture : register(t1);
sampler Sampler : register(s0);

struct PSInput {
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 WorldPos		: TEXCOORD1;
};

struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal   : NORMAL;				//法線。
	float3 Tangent  : TANGENT;				//接ベクトル。
	float2 TexCoord : TEXCOORD0;			//UV座標。
};

PSInput VSMaincreate(VSInputNmTxVcTangent In, float4x4 worldMat)
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(worldMat, In.Position);
	psInput.WorldPos = pos.xyz;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(worldMat, In.Normal));
	psInput.Tangent = normalize(mul(worldMat, In.Tangent));
	return psInput;
}

PSInput VSMain(VSInputNmTxVcTangent In)
{
	return VSMaincreate(In, mWorld);
}

float4 PSCubeMain(PSInput In) : SV_Target0
{
	float d = depthTexture.Sample(Sampler,In.TexCoord).x - (In.Position.z / In.Position.w);
	clip(d);
	float4 color = skyCubeMap.Sample(Sampler, In.Normal*-1.0f);
	return color;
}