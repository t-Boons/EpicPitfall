#include "pfpch.h"

namespace Pitfall
{
    bool TilemapCollider::Collides(const AABBCollider* other, int2& out) const
	{
		AABB aabb = other->GetAABB();

		// AABB is transformed to take care of ITileCollidable collider position.
		aabb.Min -= m_TileCollisionOffset;
		aabb.Max -= m_TileCollisionOffset;


		// Find the tiles that the AABB is in.
		const int yStart = Math::Max(Math::FloorToInt(aabb.Min.y), 0);
		const int xStart = Math::Max(Math::FloorToInt(aabb.Min.x), 0);
		const int yEnd = Math::Min(Math::CeilToInt(aabb.Max.y), Cast<int>((m_TileCollisionMap.Size.y)));
		const int xEnd = Math::Min(Math::CeilToInt(aabb.Max.x), Cast<int>((m_TileCollisionMap.Size.x)));

		for (int y = yStart; y < yEnd; y++) for (int x = xStart; x < xEnd; x++)
		{
			if (m_TileCollisionMap.IndexAt({ x, y }) > 0)
			{
				out = { x, y };
				return true;
			}
		}

		return false;
	}

	bool TilemapCollider::Overlap(const AABBCollider* other, const int2& tileIndex, float2& overlap) const
	{
		overlap = { 0, 0 };

		const float2 otherMin = other->m_AABB.Min - m_TileCollisionOffset;
		const float2 otherMax = other->m_AABB.Max - m_TileCollisionOffset;
		const float2 tileMin = { Cast<float>(tileIndex.x), Cast<float>(tileIndex.y) };
		const float2 tileMax = { Cast<float>(tileIndex.x + 1), Cast<float>(tileIndex.y + 1) };

		return Utils::CalculateOverlap(tileMin, tileMax, otherMin, otherMax, overlap);
	}
}