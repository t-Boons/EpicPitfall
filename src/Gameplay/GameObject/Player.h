#pragma once

namespace Pitfall
{
	class NumberDisplay;
	class Enemy;

	class Player : public GameObject
	{
	public:
		Player(const String& name, const float2& spawnPos);

		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		// Movement.
		void Jump(float force);
		float GetJumpForce() const { return PLAYER_JUMPSTRENGTH; };

		void Move(int direction);
		void Halt();

		// Health.
		void AttackHit();
		void Hit(const Health& entity);
		void Die();

		void AddScore(int amount);
		void RemoveScore(int amount);

		// Misc.
		void EnablePhysics(bool enabled);
		void HandOverControls(bool handedOver);
		Animator& GetAnimator() { return m_Animator; }

		// Checkpoint.
		void SetRespawnPosition(const float2& respawnPosition) { m_RespawnPosition = respawnPosition; }

		virtual void OnCollision(const CollisionResult& result) override;

	private:
		float2 m_PositionLastFrame;
		float2 m_RespawnPosition;

		NumberDisplay* m_ScoreDisplay;
		NumberDisplay* m_HealthDisplay;

		Camera* m_Camera;

		Timer m_AttackTimer;
		Timer m_ThrowTimer;
		Animator m_Animator;
		int m_LookDirection = 0;
		AudioAnimator m_Audio;
		int m_CheckpointIndex = 0;
		Health m_DynamicHealth;
		int m_Score = 0;

		bool m_Flying = false;
		bool m_Jumped = false;
		bool m_Falling = false;
		bool m_Attacking = false;
		bool m_Walking = false;
		bool m_HasControls = true;
	};
}