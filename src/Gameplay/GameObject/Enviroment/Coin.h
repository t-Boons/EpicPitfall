#pragma once

namespace Pitfall
{
	class Coin : public GameObject
	{
	public:
		Coin(const String& name, const float2& position);

		// GameObject.
		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		virtual void OnCollision(const CollisionResult& result) override;

	private:
		// Dynamic.
		Animator m_Animator;
		AudioPlayer m_Sound;
		Timer m_DestroyTimer;
		bool m_ShouldDestroy;
	};
}