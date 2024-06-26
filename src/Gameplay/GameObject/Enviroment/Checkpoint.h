#pragma once

namespace Pitfall
{
	class Checkpoint : public GameObject
	{
	public:
		Checkpoint(const String& name, const float2& position, int index);

		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		virtual void OnCollision(const CollisionResult& result) override;

		void Collect();
		void Save();
	private:
		Animator m_Animator;
		AudioPlayer m_Sound;
		Player* m_Player;
		int m_Index;
		bool m_Collected;
	};
}