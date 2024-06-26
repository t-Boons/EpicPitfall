#pragma once

namespace Pitfall
{
	class RollingPig : public Enemy
	{
	public:
		RollingPig(const String& name, const float2& position, float xMin, float xMax);

		// Gameobject.
		virtual void Start() override;
		virtual void Tick(float deltaTime) override;
		virtual void Hit(const Health& entity) override;

		// Utility function.
		bool IsInThrowingRange() const;

		// Movement.
		void AttackMelee();
		void PreAttack();
		void Move(int8_t direction);
		void Die();

	private:
		enum class RollingPigAttackState
		{
			None,
			PreAttack,
			Melee,
		};

	private:

		// Dynamic.
		Animator m_Animator;
		Timer m_AttackLoop;
		RollingPigAttackState m_AttackState;
		int8_t m_Direction = 1;
		float m_XMin;
		float m_XMax;
		Health m_Health = Health(ROLLINGPIG_HP, ROLLINGPIG_DMG);
	};
}