#include "pfpch.h"

namespace Pitfall
{
	void AABBCollider::BasicOnCollision(GameObject* object, const CollisionResult& collision)
	{
		const AABBCollider* otherObjectCollider = collision.Object->GetAABBCollider();

		if (collision.CollisionType == CollisionType::Pixel)
		{
			return;
		}

		// Ignore collisions if other object or this object is a trigger.
		if (otherObjectCollider && otherObjectCollider->m_IsTrigger || object->GetAABBCollider()->m_IsTrigger)
		{
			return;
		}

		object->SetPosition(object->GetPosition() - collision.Overlap);

		RigidBody* rb = object->GetRigidBody();

		// Resolve velocity changes due to collisions.
		if (rb)
		{
			if (rb->m_Velocity.y < 0 && collision.Overlap.y < 0 ||
				rb->m_Velocity.y > 0 && collision.Overlap.y > 0)
			{
				rb->m_Velocity.y = collision.Overlap.y;
			}


			// Tiny offset was needed to fix collision on the side when standing on the floor and side at the same time.
			// Delta time was implemented to make this work on every framerate.
			///				[o]
			///       -> [p][o]
			/// [o][o][o][o][o]
			/// 

			if (rb->m_Velocity.x < 0 && collision.Overlap.x < -Game::DeltaTime() * 2 ||
				rb->m_Velocity.x > 0 && collision.Overlap.x > 0)
			{
				rb->m_Velocity.x = collision.Overlap.x;
			}

			rb->m_Velocity = Math::ReduceFloat2(rb->m_Velocity, float2(0, 0), rb->m_Friction * Game::DeltaTime());
		}
	}

	bool AABBCollider::Collides(const AABBCollider* other) const
	{
		return m_AABB.Collides(other->GetAABB());
	}

	bool AABBCollider::Overlap(const AABBCollider* other, float2& overlap) const
	{
		return Utils::CalculateOverlap(other->m_AABB.Min, other->m_AABB.Max, m_AABB.Min, m_AABB.Max, overlap);
	}

	void AABBCollider::SetCollisionLayer(uint32_t layer)
	{
		if (layer >= 32)
		{
			return;
		}

		m_CollisionLayer = 0;
		m_CollisionLayer ^= (1 << (layer));
	}

	void AABBCollider::SetCollisionIgnoreLayer(uint32_t layer, bool enabled)
	{
		if (layer >= 32)
		{
			return;
		}

		// Resource for figuring out enabling and disabling single bits.
		// https://www.geeksforgeeks.org/how-to-turn-off-a-particular-bit-in-a-number/

		if (enabled)
		{
			m_CollisionIgnoreLayers ^= (1 << (layer));
		}
		else
		{
			m_CollisionIgnoreLayers &= ~(1 << (layer));
		}
	}

	void AABBCollider::BasicUpdateColliderPosition(GameObject* object)
	{
		object->GetAABBCollider()->m_AABB.Set(object->GetPosition());
	}
}