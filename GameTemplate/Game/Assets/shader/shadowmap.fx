/*!
 * @brief	モデルシェーダー。
 */


Texture2D<float4> albedoTexture : register(t0);
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);


//インスタンシング描画用
StructuredBuffer<float4x4> instanceMatrix : register(t100);
/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
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
// ストラクチャードバッファー
/////////////////////////////////////////////////////////////

sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal   : NORMAL;				//法線。
	float3 Tangent  : TANGENT;				//接ベクトル。
	float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
	float4 Position : SV_Position;			//頂点座標。
	float3 Normal   : NORMAL;				//法線。
	float2 TexCoord	: TEXCOORD0;			//UV座標。
	float3 Tangent	: TANGENT;				//接ベクトル。
	uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
	float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput {
	float4 Position 	: SV_POSITION;
	float2 TexCoord : TEXCOORD0;			//UV座標。
	float3 WorldPos		: TEXCOORD1;
};
/*!
 *@brief	スキン行列を計算。
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
 * @brief	スキンなしモデル用の頂点シェーダー。
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
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkincreate(VSInputNmTxWeights In, float4x4 worldMat)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	float4 pos = 0;
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
			skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
			w += In.Weights[i];
		}
		//最後のボーンを計算する。
		skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
		//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
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
