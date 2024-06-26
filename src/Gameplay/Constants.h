#pragma once

// Collision layer constants.
#define ENVIROMENT_COLLISION_LAYER 1
#define PLAYER_COLLISION_LAYER 2
#define ENEMY_COLLISION_LAYER 3

// Tile index constants.
#define TILE_SPIKE 5


// Gameplay properties //

// Score .
#define COIN_SCORE 25
#define MONKEY_DIE_SCORE 100
#define ROLLINGPIG_DIE_SCORE 75

// Player
#define PLAYER_ATTACKTIME 0.5f
#define PLAYER_JUMPSTRENGTH 40.0f
#define PLAYER_MOVEMENTSPEED 75.0f
#define PLAYER_ATTACKDISTANCE 3.0f
#define PLAYER_HP 150
#define PLAYER_DMG 10
#define PLAYER_THROWTIME 0.5f
#define PLAYER_THROWSTRRENGTH 10.0f
#define PLAYER_AUDIO_WALKSPEED 3.5f

// Projectile damage.
#define ROCK_DMG 15
#define STICK_DMG 15

#define STICK_MAXBOUNCE 3


// Special movement entities.
#define ZIPLINE_DETACHFORCE 50

#define CLIMBROPE_CLIMB_SPEED 6

#define SWINGINGROPE_SPEED 0.35f
#define SWINGINGROPE_MAXANGLE 55
#define SWINGINGROPE_DETACHFORCE 175.0f


// Enemies.
#define MONKEY_SPEED 80.0f
#define MONKEY_THROWDISTANCE 6.0f
#define MONKEY_THROWVELOCITY 10.0f
#define MONKEY_MELEEDISTANCE 2.0f
#define MONKEY_ATTACKDELAY 0.8f
#define MONKEY_ATTACKPREDELAY 0.5f

#define ROLLINGPIG_SPEED 80.0f
#define ROLLINGPIG_MELEEDISTANCE 2.0f
#define ROLLINGPIG_ATTACKDELAY 0.75f
#define ROLLINGPIG_ATTACKPREDELAY 0.25f
#define ROLLINGPIG_HP 40
#define ROLLINGPIG_DMG 15


// Effects
#define FADE_SPEED 0.5f

// Checkpoint
#define CHECKPOINT_FILEPATH "Assets/checkpoint.txt"