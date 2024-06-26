#pragma once

namespace Pitfall
{
	class Renderable;

	class PixelCollider
	{
	public:
		static void BasicUpdateColliderPosition(GameObject* object);

		bool Collides(const AABB& aabb) const;
		bool Collides(const PixelCollider* other) const;

		void Init(const Renderable* m_Renderable, const AABB& bounds);

		AABB AABBFromPixelIndex(const int2& index) const;

		void SetFlipped(bool flipped) { m_Flipped = flipped; }

	private:
		float2 m_StepSize;
		const Renderable* m_Renderable;
		AABB m_Bounds;
		bool m_Flipped = false;
	};
}