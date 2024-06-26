#include "pfpch.h"

namespace Pitfall
{

	void AABB::Update(const float2& position, const float2& size)
	{
		SetSize(size);
		Set(position);
	}

	bool AABB::Collides(const float2& otherMin, const float2& otherMax) const
	{
		// Resource for finding AABB to AABB collision
		// https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
		// https://tutorialedge.net/gamedev/aabb-collision-detection-tutorial/
		// ChatGPT

		// Check along the X-axis
		if (Max.x < otherMin.x || Min.x> otherMax.x)
		{
			return false; // No collision along the X-axis
		}

		// Check along the Y-axis
		if (Max.y < otherMin.y || Min.y> otherMax.y)
		{
			return false; // No collision along the Y-axis
		}

		// If there is no separation along any axis, the AABBs are colliding
		return true;
	}

	bool AABB::Collides(const AABB& other) const
	{
		return Collides(other.Min, other.Max);
	}

	void AABB::Set(const float2& position)
	{
		Position = position;
		Min = Position - (Size * 0.5f);
		Max = Position + (Size * 0.5f);
	}

	void AABB::SetSize(const float2& size)
	{
		Size = size;
		Min = Position - (Size * 0.5f);
		Max = Position + (Size * 0.5f);
	}
}