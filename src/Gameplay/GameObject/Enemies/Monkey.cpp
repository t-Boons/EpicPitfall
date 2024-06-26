#include "pfpch.h"

namespace Pitfall
{
	Monkey::Monkey(const String& name, const float2& position, float xMin, float xMax)
		: Enemy(name), m_XMax(xMax), m_XMin(xMin)
	{
		SetPosition(position);

		m_Renderable = new Renderable();
		m_Renderable->m_SpriteRect.Size = { 3, 3 };
		m_Renderable->m_SpriteOffset = { 0, 0.8f };

		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.SetSize({2.5f, 1.2f});
		m_AABBCollider->SetCollisionLayer(ENEMY_COLLISION_LAYER);


		// Physics.
		m_RigidBody = new RigidBody();
		m_RigidBody->m_Friction = 20;
		m_RigidBody->m_Drag = 5;
		m_RigidBody->m_Gravity = { 0, -100 };

		m_Direction = 1;

		// Animator initialization.
		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Walk", SpriteLibrary::SpriteFromSurface("Monkey_Walk"));
		m_Animator.AddAnimation("AttackMelee", SpriteLibrary::SpriteFromSurface("Monkey_Attack"));
		m_Animator.AddAnimation("Idle", SpriteLibrary::SpriteFromSurface("Monkey_Idle"));
		m_Animator.AddAnimation("Hurt", SpriteLibrary::SpriteFromSurface("Monkey_Hurt"));

		m_PixelCollider = new PixelCollider();
		m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));
	}

	void Monkey::Start()
	{
		m_Player = GameObject::FindByType<Player>();
	}

	void Monkey::Tick(float)
	{
		// Move if not in an attacking state.
		if (m_AttackType == MonkeyAttackState::None)
		{
			m_Animator.SetAnimation("Walk");
			m_Animator.SetSpeed(5);
			m_Animator.SetAnimationDirection(m_Direction);

			Move(m_Direction);

			if (GetPosition().x < m_XMin)
			{
				m_Direction = 1;
				if (m_RigidBody->m_Velocity.x < 0) m_RigidBody->m_Velocity.x = 0;
			}


			if (GetPosition().x > m_XMax)
			{
				m_Direction = -1;
				if (m_RigidBody->m_Velocity.x > 0) m_RigidBody->m_Velocity.x = 0;
			}
		}

		// If enemy is close enough attack the player.
		if (IsInThrowingRange())
		{
			m_Direction = m_Player->GetPosition().x < GetPosition().x ? -1 : 1;

			// Enable pre-attack state.
			if (m_AttackType == MonkeyAttackState::None)
			{
				PreAttack();
				m_AttackType = MonkeyAttackState::PreAttack;
			}

			// If pre-attack delay is over start to damage the player.
			if (m_AttackLoop.elapsed() > MONKEY_ATTACKPREDELAY)
			{
				m_AttackType = MonkeyAttackState::Melee;
			}

			// Damage the player each time this loops and throw or hit depending on range..
			if (m_AttackLoop.elapsed() > MONKEY_ATTACKDELAY)
			{
				if (IsInMeleeRange())
				{
					AttackMelee();
				}
				else
				{
					AttackThrow();
				}

				m_AttackLoop.reset();
				m_AttackType = MonkeyAttackState::None;
			}
		}
		else
		{
			m_AttackLoop.reset();
			m_AttackType = MonkeyAttackState::None;
		}

		m_Animator.Tick();
	}

	void Monkey::Hit(const Health& entity)
	{
		m_Health.HP -= entity.ATK;

		Print("Monkey HP Remaining: ");
		PrintInt(m_Health.HP);

		if (m_Health.HP <= 0)
		{
			Die();
		}

		m_Animator.SetAnimation("Hurt", false);
		m_Animator.SetSpeed(7);
		m_Animator.OneShot();
	}

	void Monkey::Die()
	{
		m_Player->AddScore(MONKEY_DIE_SCORE);
		Destroy(this);
	}

	bool Monkey::IsInThrowingRange() const
	{
		return Math::Length(m_Player->GetPosition() - GetPosition()) < MONKEY_THROWDISTANCE;
	}

	bool Monkey::IsInMeleeRange() const
	{
		return Math::Length(m_Player->GetPosition() - GetPosition()) < MONKEY_MELEEDISTANCE;
	}

	void Monkey::PreAttack()
	{
		m_Animator.SetAnimation("Idle");
		m_Animator.SetAnimationDirection(m_Direction);
		m_Animator.SetSpeed(6);
	}

	void Monkey::AttackMelee()
	{
		m_Animator.SetAnimation("AttackMelee");
		m_Animator.SetAnimationDirection(m_Direction);
		m_Animator.SetSpeed(6);
		m_Player->Hit(m_Health);
	}

	void Monkey::AttackThrow()
	{
		Instantiate(new Stick("Stick", this, GetPosition(),
			(Math::Normalize(m_Player->GetPosition() - GetPosition()) + float2(0, 1.2f)) * MONKEY_THROWVELOCITY));
	}

	void Monkey::Move(int8_t direction)
	{
		m_PixelCollider->SetFlipped(direction < 0);
		m_RigidBody->m_Velocity += float2(direction, 0) * MONKEY_SPEED * Game::DeltaTime();
	}
}