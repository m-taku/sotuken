
TextureCube<float4> skyCubeMap : register(t0);	//�X�J�C�L���[�u�}�b�v�B
sampler Sampler : register(s0);
cbuffer VSPSCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
struct PSInput {
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 WorldPos		: TEXCOORD1;
};

struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal   : NORMAL;				//�@���B
	float3 Tangent  : TANGENT;				//�ڃx�N�g���B
	float2 TexCoord : TEXCOORD0;			//UV���W�B
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
	float4 color = skyCubeMap.Sample(Sampler, In.Normal);
	return color;
}