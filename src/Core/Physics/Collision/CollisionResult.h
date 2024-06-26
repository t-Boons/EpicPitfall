#pragma once

#define ENERGY_LOSS_MULTIPLIER 0.5f

namespace Pitfall
{
	class GameObject;

	enum class CollisionType
	{
		Tile, AABB, Pixel
	};

	
	struct CollisionResult
	{
	public:
		float2 Overlap;
		float2 Normal;
		GameObject* Object;
		CollisionType CollisionType;
		int TileIndex;
		bool IsTrigger;
	};
}