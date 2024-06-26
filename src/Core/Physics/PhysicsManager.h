#pragma once

#define MAX_COLLIDERS 256
#define MAX_COLLISION_SOLVE_ATTEMPTS 4

namespace Pitfall
{
	class GameObject;
	class TilemapCollider;
	class AABBCollider;
	class RigidBody;
	struct CollisionResult;

	class PhysicsManager
	{
	public:
		PhysicsManager()
			: m_PhysicsObjectCount(0)
		{}

		static void Add(GameObject* object);
		static void Remove(GameObject* object);

		static void Reset();

		static void ResolveCollisions();
		static void ResolveVelocities();
		static bool CompareAABBToTile(const AABBCollider* square, const TilemapCollider* tile, CollisionResult& result);
		static bool CompareAABBAABB(const AABBCollider* square1, const AABBCollider* square2, CollisionResult& result);
		static bool ComparePixelPixel(const PixelCollider* pixel1, const PixelCollider* pixel2, CollisionResult& result);

	private:
		static PhysicsManager* s_Instance;
		StaticList<GameObject*, MAX_COLLIDERS> m_PhysicsObjects;
		uint32_t m_PhysicsObjectCount;
	};
}