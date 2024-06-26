#pragma once

#define TILEMAP_COLLISION_LAYER 0b01 // Layer 1 in binary

namespace Pitfall
{
	class TilemapCollider
	{
	public:
		bool Collides(const AABBCollider* other, int2& out) const;
		bool Overlap(const AABBCollider* other, const int2& tileIndex, float2& overlap) const;

		const int2& GetCollisionOffset() { return m_TileCollisionOffset; }
		const TileChunk& GetTileMap() { return m_TileCollisionMap; }

	public:
		int2 m_TileCollisionOffset{};
		TileChunk m_TileCollisionMap;
	};
}