/*!
 * @brief	���f���V�F�[�_�[�B
 */


Texture2D<float4> albedoTexture : register(t0);
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);


//�C���X�^���V���O�`��p
StructuredBuffer<float4x4> instanceMatrix : register(t100);
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
cbuffer NatureCB : register(b1) {
	float4 windowdir;
	float4 playerpos;
	float power;
	float maxhight;
};
cbuffer BeforeShadowCB : register(b5) {
	float4x4 beforeLightView[3];
	float4x4 beforeLightProj[3];
	int No;
}
/////////////////////////////////////////////////////////////
// �X�g���N�`���[�h�o�b�t�@�[
/////////////////////////////////////////////////////////////

sampler Sampler : register(s0);

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
	float2 TexCoord : TEXCOORD0;			//UV���W�B
	float3 WorldPos		: TEXCOORD1;
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
PSInput VSTree(VSInputNmTxVcTangent In, float4x4 worldMat)
{
	PSInput psInput = (PSInput)0;
	float3 zero = float3(0.0f, 0.0f, 0.0f);

	float3 sub = In.Position.xyz - zero;
	float hight = max(sub.z, 0.0f);
	float t = pow((hight / maxhight), 3.0f)*power;

	float4 pos = mul(worldMat, In.Position);
	float3 toPlayer = pos - playerpos;
	toPlayer.y = 0.0f;
	float len = length(toPlayer);

	toPlayer = normalize(toPlayer);
	float maxlen = 100.0f;
	float s = abs(min(len, maxlen) - maxlen)*pow((hight / maxhight), 3.0f);

	pos.xyz += (windowdir*10.0f * t) + (toPlayer*s);
	psInput.WorldPos = pos.xyz;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);

	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
PSInput VSTreeMainInstancing(VSInputNmTxVcTangent In, uint instanceID : SV_InstanceID)
{
	return VSTree(In, instanceMatrix[instanceID]);
}
PSInput VSTreeMain(VSInputNmTxVcTangent In)
{
	return VSTree(In, mWorld);
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
	psInput.WorldPos = pos.xyz;
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

float PSMain(PSInput In) : SV_Target0
{
	float min,max;
	max = 0.9985f;
	min = 0.0015f;
	if (No == 0)
	{
		return In.Position.z;
	}
	if (No == 1)
	{
		float4 World = float4(In.WorldPos, 1.0f);
		float4 LUV;
		LUV = mul(beforeLightView[0], World);
		LUV = mul(beforeLightProj[0], LUV);
		LUV.xyz /= LUV.w;
		LUV.xy *= float2(0.5f, -0.5f);
		LUV.xy += 0.5f;
		if (
			LUV.x < max
			&& LUV.x > min
			&& LUV.y < max
			&& LUV.y > min
			&& LUV.z < max
			&& LUV.z > min
			)
		{
			return 1.0f;
		}
	}

	if (No == 2)
	{
		float4 World = float4(In.WorldPos, 1.0f);
		float4 LUV;
		LUV = mul(beforeLightView[0], World);
		LUV = mul(beforeLightProj[0], LUV);
		LUV.xyz /= LUV.w;
		LUV.xy *= float2(0.5f, -0.5f);
		LUV.xy += 0.5f;
		if (
			LUV.x < max
			&& LUV.x > min
			&& LUV.y < max
			&& LUV.y > min
			&& LUV.z < max
			&& LUV.z > min
			)
		{
			return 1.0f;
		}

		LUV = mul(beforeLightView[1], World);
		LUV = mul(beforeLightProj[1], LUV);
		LUV.xyz /= LUV.w;
		LUV.xy *= float2(0.5f, -0.5f);
		LUV.xy += 0.5f;
		if (
			LUV.x < max
			&& LUV.x > min
			&& LUV.y < max
			&& LUV.y > min
			&& LUV.z < max
			&& LUV.z > min
			)
		{
			return 1.0f;
		}
	}

	return In.Position.z;
}

float PSTreeMain(PSInput In) : SV_Target0
{
	float a = albedoTexture.Sample(Sampler, In.TexCoord).w;
	clip(a - 0.1f);
	float min, max;
	max = 0.9985f;
	min = 0.0015f;
	if (No == 0)
	{
		return In.Position.z;
	}
	if (No == 1)
	{
		float4 World = float4(In.WorldPos, 1.0f);
		float4 LUV;
		LUV = mul(beforeLightView[0], World);
		LUV = mul(beforeLightProj[0], LUV);
		LUV.xyz /= LUV.w;
		LUV.xy *= float2(0.5f, -0.5f);
		LUV.xy += 0.5f;
		if (
			LUV.x < max
			&& LUV.x > min
			&& LUV.y < max
			&& LUV.y > min
			&& LUV.z < max
			&& LUV.z > min
			)
		{
			return 1.0f;
		}
	}

	if (No == 2)
	{
		float4 World = float4(In.WorldPos, 1.0f);
		float4 LUV;
		LUV = mul(beforeLightView[0], World);
		LUV = mul(beforeLightProj[0], LUV);
		LUV.xyz /= LUV.w;
		LUV.xy *= float2(0.5f, -0.5f);
		LUV.xy += 0.5f;
		if (
			LUV.x < max
			&& LUV.x > min
			&& LUV.y < max
			&& LUV.y > min
			&& LUV.z < max
			&& LUV.z > min
			)
		{
			return 1.0f;
		}

		LUV = mul(beforeLightView[1], World);
		LUV = mul(beforeLightProj[1], LUV);
		LUV.xyz /= LUV.w;
		LUV.xy *= float2(0.5f, -0.5f);
		LUV.xy += 0.5f;
		if (
			LUV.x < max
			&& LUV.x > min
			&& LUV.y < max
			&& LUV.y > min
			&& LUV.z < max
			&& LUV.z > min
			)
		{
			return 1.0f;
		}
	}

	return In.Position.z;
}
