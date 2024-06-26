#pragma once

// jBikker template pch header.
#include "precomp.h"

// Third party libs.

// Resource for learning lib:
// https://github.com/nlohmann/json
#include "lib/nlohmann/json.hpp"


// Audio library struggles:
/*
I tried using
https://www.un4seen.com/
But it gave me ambiguous type errors.

I tried using
https://github.com/ddiakopoulos/libnyquist
But I had trouble with importing it and it seemed too complex for my usecase.

I had an issue with the miniaudio lib because of some missing Winapi stuff. Probably because I am using Win11.

I ended up using:
https://github.com/jpvanoosten/Audio
It is a simple audio library that does everything I need for this project.
*/
#include "lib/audio/inc/Audio/Sound.hpp" 


// Core defines.
#include "Core/Core.h"
#include "Gameplay/Constants.h"

// Container headers
#include "Core/Containers/CustomString.h"
#include "Core/Containers/StaticList.h"
#include "Core/Containers/DynamicList.h"

// Core headers.
#include "Core/Game.h"
#include "Core/DataSerializer.h"
#include "Core/File.h"
#include "Core/SpriteLibrary.h"
#include "Core/CustomMath.h"
#include "Core/Rect.h"

#include "Core/Physics/Collision/AABB.h"
#include "Core/Physics/Collision/CollisionResult.h"
#include "Core/Physics/Collision/AABBCollider.h"
#include "Core/Physics/Collision/TilemapCollider.h"
#include "Core/Physics/Collision/PixelCollider.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/RigidBody.h"

#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Renderable.h"
#include "Core/Rendering/NumberRenderer.h"

// Gameplay:

// Gameplay base classes.
#include "Gameplay/GameObject/GameObject.h"
#include "Gameplay/World/World.h"

// Utility classes.
#include "Gameplay/Utils/Health.h"
#include "Gameplay/Utils/Animator.h"
#include "Gameplay/Utils/AudioPlayer.h"
#include "Gameplay/Utils/AudioAnimator.h"

// Levels.
#include "Gameplay/World/Level01.h"
#include "Gameplay/World/TitleScreen.h"
#include "Gameplay/World/EndScreen.h"

// Gameplay objects.
#include "Gameplay/GameObject/Player.h"
#include "Gameplay/GameObject/Camera.h"

#include "Gameplay/GameObject/Enviroment/Enviroment.h"
#include "Gameplay/GameObject/Enviroment/EnviromentCollision.h"
#include "Gameplay/GameObject/Enviroment/SwingingRope.h"
#include "Gameplay/GameObject/Enviroment/Zipline.h"
#include "Gameplay/GameObject/Enviroment/ClimbRope.h"
#include "Gameplay/GameObject/Enviroment/Coin.h"
#include "Gameplay/GameObject/Enviroment/Finish.h"
#include "Gameplay/GameObject/Enviroment/Spiderweb.h"
#include "Gameplay/GameObject/Enviroment/Checkpoint.h"

#include "Gameplay/GameObject/Enemies/Enemy.h"
#include "Gameplay/GameObject/Enemies/RollingPig.h"
#include "Gameplay/GameObject/Enemies/Monkey.h"

#include "Gameplay/GameObject/Projectiles/Rock.h"
#include "Gameplay/GameObject/Projectiles/Stick.h"

#include "Gameplay/GameObject/Enviroment/BoxCollider.h"

// UI
#include "Gameplay/GameObject/UI/Button.h"
#include "Gameplay/GameObject/UI/StartButton.h"
#include "Gameplay/GameObject/UI/BackToMenuButton.h"
#include "Gameplay/GameObject/UI/Image.h"
#include "Gameplay/GameObject/UI/NumberDisplay.h"
#include "Gameplay/GameObject/UI/ParralaxBackground.h"