#pragma once

namespace Pitfall
{
	class Rock : public GameObject
	{
	public:
		Rock(const String& name, GameObject* thrower, const float2& initialPosition, const float2& movementDirection);

		virtual void Start() override;
		virtual void Tick(float) override;

		virtual void OnCollision(const CollisionResult& result) override;

	private:
		GameObject* m_Thrower;
		Animator m_Animator;
		Timer m_Lifetime;
		float2 m_MovementDirection;
		int m_Bounces;
	};
}