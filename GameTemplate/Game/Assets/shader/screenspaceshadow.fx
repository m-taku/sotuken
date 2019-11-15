


/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////

cbuffer ShadowCB:register(b0) {
	float4x4 LightView;
	float4x4 LightProj;
}


struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};
/*!
 * @brief	ピクセルシェーダーの入力。
 */
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

float PSMain(PSInput In) : SV_Target0
{

	return In.Position.z;
}
