#pragma once

namespace Pitfall
{
	class AABBCollider
	{
	public:
		static void BasicOnCollision(GameObject* object, const CollisionResult& collision);
		static void BasicUpdateColliderPosition(GameObject* object);

		const AABB& GetAABB() const { return m_AABB; }
		bool IsStatic() const { return m_IsStatic; }
		bool IsTrigger() const { return m_IsTrigger; }

		bool Collides(const AABBCollider* other) const;
		bool Overlap(const AABBCollider* other, float2& overlap) const;

		void SetCollisionLayer(uint32_t layer);
		void SetCollisionIgnoreLayer(uint32_t layer, bool enabled);

		uint32_t GetCollisionLayer() const { return m_CollisionLayer; }
		uint32_t GetCollissionIgnoreLayers() const { return m_CollisionIgnoreLayers; }

	public:
		bool m_IsStatic = false;
		bool m_IsTrigger = false;
		AABB m_AABB;

	private:
		uint32_t m_CollisionLayer = (1 << ENVIROMENT_COLLISION_LAYER); // Set Tilemap collision layer by default.
		uint32_t m_CollisionIgnoreLayers = 0; // Set Tilemap collision layer by default.
	};
}