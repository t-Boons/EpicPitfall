#pragma once

namespace Pitfall
{
	// Having the AABB struct also store a Position and Size will make it easier to use and manipulate.
	// It does cost a bit more memory but since the game does not have that many AABB's its worth it.

	struct AABB
	{
	public:
		void Update(const float2& position, const float2& size);
		bool Collides(const float2& otherMin, const float2& otherMax) const;
		bool Collides(const AABB& other) const;

		void Set(const float2& position);
		void SetSize(const float2& size);

	public:
		float2 Size{};
		float2 Position{};
		float2 Min{};
		float2 Max{};
	};
}