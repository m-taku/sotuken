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
#include "graphics/GraphicsEngine.h"
#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"
#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Primitive.h"


#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"
#include "physics/Physics.h"
#include "character/CharacterController.h"

#include "GameObject/IGameObjectManager.h"
#include "GameTime/GameTime.h"
#include "GameFont/GameFont.h"

#include "GameCamera.h"

const float FRAME_BUFFER_W = 1920.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 1080.0f;				//フレームバッファの高さ。
using namespace smEngine;

static const int MAX_BONE = 512;	//!<ボーンの最大数。
