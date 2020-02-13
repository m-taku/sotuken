/*!
 * @brief	���f���V�F�[�_�[�B
 */


 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
 //�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

Texture2D<float4> normalMap : register(t2);

//�C���X�^���V���O�`��p
StructuredBuffer<float4x4> instanceMatrix : register(t100);
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer ShadowCB:register(b0) {
	float4x4 LightView;
	float4x4 LightProj;
}
/////////////////////////////////////////////////////////////
// �X�g���N�`���[�h�o�b�t�@�[
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal   : NORMAL;				//�@���B
	float3 Tangent  : TANGENT;				//�ڃx�N�g���B
	float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
	float4 Position : SV_Position;			//���_���W�B
	float3 Normal   : NORMAL;				//�@���B
	float2 TexCoord	: TEXCOORD0;			//UV���W�B
	float3 Tangent	: TANGENT;				//�ڃx�N�g���B
	uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
	float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput {
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 WorldPos		: TEXCOORD1;
};

struct PSOutput {
	float4 diffuse : SV_Target0;
	float4 normal  : SV_Target1;
	float4 world   : SV_Target2;
	float4 depth   : SV_Target3;
	float4 shadow  : SV_Target4;
	float4 silhouette:SV_Target5;
};
/*!
 *@brief	�X�L���s����v�Z�B
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
		w += In.Weights[i];
	}

	skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	return skinning;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
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
PSInput VSMainInstancing(VSInputNmTxVcTangent In, uint instanceID : SV_InstanceID)
{
	return VSMaincreate(In, instanceMatrix[instanceID]);
}
PSInput VSMain(VSInputNmTxVcTangent In)
{
	return VSMaincreate(In, mWorld);
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkincreate(VSInputNmTxWeights In, float4x4 worldMat)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
			skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
			w += In.Weights[i];
		}
		//�Ō�̃{�[�����v�Z����B
		skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
		//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
		//pos = mul(skinning, In.Position);
	}
	pos = mul(skinning, In.Position);
	pos = mul(worldMat, pos);
	float4x4 normal = 0;
	normal = mul(worldMat, skinning);
	psInput.Normal = normalize(mul(normal, In.Normal));
	psInput.Tangent = normalize(mul(skinning, In.Tangent));
	psInput.WorldPos = pos;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
PSInput VSMainSkinInstancing(VSInputNmTxWeights In, uint instanceID : SV_InstanceID)
{
	return VSMainSkincreate(In, instanceMatrix[instanceID]);
}
PSInput VSMainSkin(VSInputNmTxWeights In)
{
	return VSMainSkincreate(In, mWorld);
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
PSOutput PSMain(PSInput In)
{
	PSOutput psout;
	psout.diffuse = 0.0f;
	psout.normal = 0.0f;
	psout.world = 0.0f;
	psout.depth = 1.0f;
	psout.shadow = 1.0f;
	psout.silhouette = 0.0f;


	psout.diffuse = albedoTexture.Sample(Sampler, In.TexCoord);
	float3 normal;
	//�@���Ɛڃx�N�g���̊O�ς��v�Z���āA�]�x�N�g�����v�Z����B
	float3 biNormal = cross(In.Normal, In.Tangent);
	biNormal = normalize(biNormal);

	normal = normalMap.Sample(Sampler, In.TexCoord).xyz;

	//0.0�`1.0�͈̔͂ɂȂ��Ă���^���W�F���g�X�y�[�X�@����
	//-1.0�`1.0�͈̔͂ɕϊ�����B
	normal = (normal * 2.0f) - 1.0f;
	//�@�����^���W�F���g�X�y�[�X����A���[���h�X�y�[�X�ɕϊ�����B
	normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
	normal = normalize(normal);
	psout.normal =float4(normal, 1.0f);
	psout.world = float4(In.WorldPos.xyz, 1.0f);
	psout.depth = In.Position.z;
	psout.silhouette = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return psout;
}

PSOutput PSTreeMain(PSInput In)
{
	PSOutput psout;
	psout.diffuse = 0.0f;
	psout.normal = 0.0f;
	psout.world = 0.0f;
	psout.depth = 1.0f;
	psout.shadow = 1.0f;
	psout.silhouette = 0.0f;

	psout.diffuse = albedoTexture.Sample(Sampler, In.TexCoord);
	clip(psout.diffuse.w - 0.1f);

	float3 normal;
	//�@���Ɛڃx�N�g���̊O�ς��v�Z���āA�]�x�N�g�����v�Z����B
	float3 biNormal = cross(In.Normal, In.Tangent);
	biNormal = normalize(biNormal);

	normal = normalMap.Sample(Sampler, In.TexCoord).xyz;

	//0.0�`1.0�͈̔͂ɂȂ��Ă���^���W�F���g�X�y�[�X�@����
	//-1.0�`1.0�͈̔͂ɕϊ�����B
	normal = (normal * 2.0f) - 1.0f;
	//�@�����^���W�F���g�X�y�[�X����A���[���h�X�y�[�X�ɕϊ�����B
	normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
	normal = normalize(normal);
	psout.normal = float4(normal, 1.0f);
	psout.world = float4(In.WorldPos.xyz, 1.0f);
	psout.depth = In.Position.z;
	psout.silhouette = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return  psout;
}