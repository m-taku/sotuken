#pragma once

struct SDirectionLight
{
	CVector4 color;
	CVector3 direction;
};
struct SPointLight
{
	CVector4 color;
	CVector4 attn;
	CVector3 position;
};
struct SSpotLight
{
	CVector4 color;
	CVector4 attn;
	CVector3 position;
	CVector3 direction;
};

