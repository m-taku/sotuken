/*!
 * @brief	モデルシェーダー。
 */


 /////////////////////////////////////////////////////////////
 // Shader Resource View
 /////////////////////////////////////////////////////////////
 //アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

Texture2D<float4> normalMap : register(t2);

//インスタンシング描画用
StructuredBuffer<float4x4> instanceMatrix : register(t100);
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

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

cbuffer ShadowCB:register(b0) {
	float4x4 LightView;
	float4x4 LightProj;
}
/////////////////////////////////////////////////////////////
// ストラクチャードバッファー
/////////////////////////////////////////////////////////////


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
// ピクセルシェーダーのエントリ関数。
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
	//法線と接ベクトルの外積を計算して、従ベクトルを計算する。
	float3 biNormal = cross(In.Normal, In.Tangent);
	biNormal = normalize(biNormal);

	normal = normalMap.Sample(Sampler, In.TexCoord).xyz;

	//0.0～1.0の範囲になっているタンジェントスペース法線を
	//-1.0～1.0の範囲に変換する。
	normal = (normal * 2.0f) - 1.0f;
	//法線をタンジェントスペースから、ワールドスペースに変換する。
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
	//法線と接ベクトルの外積を計算して、従ベクトルを計算する。
	float3 biNormal = cross(In.Normal, In.Tangent);
	biNormal = normalize(biNormal);

	normal = normalMap.Sample(Sampler, In.TexCoord).xyz;

	//0.0～1.0の範囲になっているタンジェントスペース法線を
	//-1.0～1.0の範囲に変換する。
	normal = (normal * 2.0f) - 1.0f;
	//法線をタンジェントスペースから、ワールドスペースに変換する。
	normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
	normal = normalize(normal);
	psout.normal = float4(normal, 1.0f);
	psout.world = float4(In.WorldPos.xyz, 1.0f);
	psout.depth = In.Position.z;
	psout.silhouette = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return  psout;
}