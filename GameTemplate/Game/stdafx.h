#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTKのインクルードファイル。
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "Noncopyable/Noncopyable.h"
#include "HID/Pad.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/Shader.h"
#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Primitive.h"
#include "graphics/RenderTarget.h"
#include "graphics/PostEffect/PostEffect.h"
#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"
#include "graphics/GaussianBlur.h"
#include "graphics/Bloom.h"
#include "graphics/ToneMap.h"
#include "graphics/Shadow/ShadowMap.h"
#include "graphics/Shadow/ShadowManager.h"
#include "graphics/Light/Light.h"
#include "graphics/Light/LightBase.h"
#include "graphics/Light/DirectionLight.h"
#include "graphics/Light/PointLight.h"
#include "graphics/Light/LightManager.h"
#include "graphics/DeferredRender.h"


#include "graphics/GraphicsEngine.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"
#include "physics/Physics.h"
#include "Physics/CollisionAttr.h"
#include "character/CharacterController.h"

#include "graphics/Sprite/Sprite.h"

#include "GameObject/IGameObjectManager.h"
#include "GameTime/GameTime.h"
#include "GameFont/GameFont.h"

#include "GameCamera.h"
#define DenugWorld
enum EBufferSlot
{
	enLightParam_Slot_No = 1,
	enDerectionLightSB_Slot_No = 100,
	enPointLightSB_Slot_No,
	enSpotLightSB_Slot_No
};

const float FRAME_BUFFER_W = 1920.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 1080.0f;				//フレームバッファの高さ。
using namespace smEngine;
static int momomomo = 0;
static const int MAX_BONE = 512;	//!<ボーンの最大数。
