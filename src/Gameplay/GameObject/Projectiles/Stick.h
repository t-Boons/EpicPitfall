#pragma once

namespace Pitfall
{
	class Stick : public GameObject
	{
	public:
		Stick(const String& name, GameObject* thrower, const float2& initialPosition, const float2& velocity);

		virtual void Start() override;
		virtual void Tick(float) override;

		virtual void OnCollision(const CollisionResult& result) override;

	private:
		GameObject* m_Thrower;
		Animator m_Animator;
	};
}