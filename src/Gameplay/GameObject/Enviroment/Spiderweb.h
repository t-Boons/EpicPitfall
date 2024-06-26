#pragma once

namespace Pitfall
{
	class Spiderweb : public GameObject
	{
	public:
		Spiderweb(const String& name, const  int2& position, float bounceStrength);

		virtual void Tick(float) override;

		virtual void OnCollision(const CollisionResult& result) override;

	private:
		float m_BounceStrength;
		Timer m_BounceResetTimer;
		Animator m_Animator;
		AudioAnimator m_Audio;
	};
}