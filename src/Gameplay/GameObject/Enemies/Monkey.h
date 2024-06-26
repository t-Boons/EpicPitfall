#pragma once

namespace Pitfall
{
	class Monkey : public Enemy
	{
	public:
		Monkey(const String& name, const float2& position, float xMin, float xMax);

		// Gameobject.
		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		virtual void Hit(const Health& entity) override;

		// Utility functions.
		bool IsInThrowingRange() const;
		bool IsInMeleeRange() const;

		// Movement.
		void AttackMelee();
		void AttackThrow();
		void PreAttack();
		void Move(int8_t direction);
		void Die();

	private:
		enum class MonkeyAttackState
		{
			None,
			PreAttack,
			Throw,
			Melee,
		};

	private:

		// Dynamic.
		Animator m_Animator;
		Timer m_AttackLoop;
		int8_t m_Direction = 1;
		MonkeyAttackState m_AttackType = MonkeyAttackState::None;

		// Properties
		Health m_Health = Health(20, 10);
		float m_XMin;
		float m_XMax;
	};
}