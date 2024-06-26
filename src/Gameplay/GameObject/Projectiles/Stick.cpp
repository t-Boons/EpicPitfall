#include "pfpch.h"

namespace Pitfall
{
	Stick::Stick(const String& name, GameObject* thrower, const float2& initialPosition, const float2& velocity)
		: GameObject(name)
	{
		SetPosition(initialPosition);

		m_RigidBody = new RigidBody();
		m_RigidBody->m_Velocity = velocity;
		m_RigidBody->m_Gravity = { 0, -25 };
		m_RigidBody->m_Drag = 0.5f;

		m_Renderable = new Renderable();
		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Stick", SpriteLibrary::SpriteFromSurface("Throwable_Stick"));

		m_PixelCollider = new PixelCollider();
		m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));

		m_Thrower = thrower;
	}

	void Stick::Start()
	{
		m_Animator.SetAnimation("Stick");
		m_Animator.SetSpeed(7.4f);
		m_Animator.SetAnimationDirection(m_RigidBody->m_Velocity.x < 0 ? -1 : 1);
	}

	void Stick::Tick(float)
	{
		m_Animator.Tick();
	}

	void Stick::OnCollision(const CollisionResult& result)
	{
 		if (result.Object != m_Thrower)
		{
			Player* p = TryCast<Player*>(result.Object);

			if (p)
			{
				p->Hit(Health(0, STICK_DMG));
				Destroy(this);
			}

			if (!result.IsTrigger)
			{
				Destroy(this);
			}
		}
	}
}