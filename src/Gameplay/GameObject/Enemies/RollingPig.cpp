#include "pfpch.h"

namespace Pitfall
{
	RollingPig::RollingPig(const String& name, const float2& position, float xMin, float xMax)
		: Enemy(name), m_AttackState(RollingPigAttackState::None), m_XMax(xMax), m_XMin(xMin)
	{
		SetPosition(position);

		m_Renderable = new Renderable();
		m_Renderable->m_SpriteRect.Size = { 3, 3 };
		m_Renderable->m_SpriteOffset = { 0, 0.95f };


		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.SetSize({ 2.5f, 1.2f });
		m_AABBCollider->SetCollisionLayer(ENEMY_COLLISION_LAYER);


		// Physics.
		m_RigidBody = new RigidBody();
		m_RigidBody->m_Friction = 20;
		m_RigidBody->m_Drag = 5;
		m_RigidBody->m_Gravity = { 0, -100 };

		m_Direction = 1;

		// Animator initialization.
		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Walk", SpriteLibrary::SpriteFromSurface("RollingPig_Walk"));
		m_Animator.AddAnimation("AttackMelee", SpriteLibrary::SpriteFromSurface("RollingPig_Attack"));
		m_Animator.AddAnimation("Idle", SpriteLibrary::SpriteFromSurface("RollingPig_Idle"));
		m_Animator.AddAnimation("Hurt", SpriteLibrary::SpriteFromSurface("RollingPig_Hurt"));

		m_PixelCollider = new PixelCollider();
		m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));
	}

	void RollingPig::Start()
	{
		m_Player = GameObject::FindByType<Player>();
	}

	void RollingPig::Tick(float)
	{
		// Move if not in an attacking state.
		if (m_AttackState == RollingPigAttackState::None)
		{
			m_Animator.SetAnimation("Walk");
			m_Animator.SetSpeed(5);
			m_Animator.SetAnimationDirection(m_Direction);

			Move(m_Direction);
			m_PixelCollider->SetFlipped(m_Direction < 0);

			if (GetPosition().x < m_XMin + m_AABBCollider->m_AABB.Size.x)
			{
				m_Direction = 1;
				if (m_RigidBody->m_Velocity.x < 0) m_RigidBody->m_Velocity.x = 0;
			}


			if (GetPosition().x > m_XMax - m_AABBCollider->m_AABB.Size.x)
			{
				m_Direction = -1;
				if (m_RigidBody->m_Velocity.x > 0) m_RigidBody->m_Velocity.x = 0;
			}
		}

		// If enemy is close enough attack the player.
		if (IsInThrowingRange())
		{
			m_Direction = m_Player->GetPosition().x < GetPosition().x ? -1 : 1;

			if (m_AttackState == RollingPigAttackState::None)
			{
				PreAttack();
				m_AttackState = RollingPigAttackState::PreAttack;
			}

			// If pre-attack delay is over start to damage the player.
			if (m_AttackLoop.elapsed() > ROLLINGPIG_ATTACKPREDELAY)
			{
				m_AttackState = RollingPigAttackState::Melee;
			}

			// Damage the player each time this loops.
			if (m_AttackLoop.elapsed() > ROLLINGPIG_ATTACKDELAY)
			{
				AttackMelee();
				m_AttackLoop.reset();
			}
		}
		else
		{
			m_AttackLoop.reset();
			m_AttackState = RollingPigAttackState::None;
		}

		m_Animator.Tick();
	}

	void RollingPig::Hit(const Health& entity)
	{
		// Play hurt anim.
		m_Health.HP -= entity.ATK;

		Print("RollingPig HP Remaining: ");
		PrintInt(m_Health.HP);

		m_Animator.SetAnimation("Hurt");
		m_Animator.SetSpeed(7.5f);
		m_Animator.OneShot();

		if (m_Health.HP <= 0)
		{
			Die();
		}
	}

	void RollingPig::Die()
	{
		m_Player->AddScore(ROLLINGPIG_DIE_SCORE);
		Destroy(this);
	}

	bool RollingPig::IsInThrowingRange() const
	{
		return Math::Length(m_Player->GetPosition() - GetPosition()) < ROLLINGPIG_MELEEDISTANCE;
	}

	void RollingPig::PreAttack()
	{
		m_Animator.SetAnimation("Idle");
		m_Animator.SetAnimationDirection(m_Direction);
		m_Animator.SetSpeed(6);
	}

	void RollingPig::AttackMelee()
	{
		m_Animator.SetAnimation("AttackMelee");
		m_Animator.SetAnimationDirection(m_Direction);
		m_Animator.SetSpeed(6);
		m_Player->Hit(m_Health);
	}

	void RollingPig::Move(int8_t direction)
	{
		m_RigidBody->m_Velocity += float2(direction, 0) * ROLLINGPIG_SPEED * Game::DeltaTime();
	}
}