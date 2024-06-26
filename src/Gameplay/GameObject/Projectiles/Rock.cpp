#include "pfpch.h"

namespace Pitfall
{
	Rock::Rock(const String& name, GameObject* thrower, const float2& initialPosition, const float2& movementDirection)
		: GameObject(name), m_Bounces(0)
	{
		SetPosition(initialPosition);
		m_MovementDirection = movementDirection;

		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.SetSize(0.5f);
		m_AABBCollider->m_IsTrigger = true;

		m_Renderable = new Renderable();
		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Rock_Throw", SpriteLibrary::SpriteFromSurface("Rock"));

		m_PixelCollider = new PixelCollider();
		m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));

		m_RigidBody = new RigidBody();

		m_Thrower = thrower;
	}

	void Rock::Start()
	{
		m_Animator.SetAnimation("Rock_Throw");
		m_Animator.SetSpeed(7.4f);
		m_Animator.SetAnimationDirection(m_MovementDirection.x < 0 ? -1 : 1);

		m_RigidBody->m_Velocity = m_MovementDirection;
	}

	void Rock::Tick(float)
	{
		m_Animator.Tick();

		if (m_Lifetime.elapsed() > 5.0f)
		{
			Destroy(this);
		}
	}

	void Rock::OnCollision(const CollisionResult& result)
	{
		if (result.Object != m_Thrower)
		{
			Enemy* e = TryCast<Enemy*>(result.Object);

			if (e)
			{
				e->Hit(Health(0, ROCK_DMG));
				Destroy(this);
			}

			if (m_Bounces > STICK_MAXBOUNCE)
			{
				Destroy(this);
			}

			if (!result.IsTrigger)
			{
				m_RigidBody->m_Velocity = Math::Reflect(m_RigidBody->m_Velocity * 0.9f, result.Normal);
				m_Bounces++;
			}
		}

		AABBCollider::BasicOnCollision(this, result);
	}
}