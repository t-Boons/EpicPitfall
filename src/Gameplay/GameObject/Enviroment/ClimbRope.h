#pragma once

namespace Pitfall
{
	class ClimbRope : public GameObject
	{
	public:		 
		ClimbRope(const String& name, const int2& position, int height);

		// Utility functions.
		float2 GetPositionFromPercentage(float percentage) const { return Math::LerpFloat2(m_BottomPosition, m_TopPosition, percentage); }
		float GetPercentageFromPosition(const float2& position) const { return Math::InvLerp(m_BottomPosition.y, m_TopPosition.y, position.y); }
		bool PlayerOnTopOfLadder() const;

		// GameObject.
		void Tick(float deltaTime) override;
		void Start() override;

		void OnCollision(const CollisionResult& collision);

		void SetIsTrigger(bool enabled) { m_AABBCollider->m_IsTrigger = enabled; }

		// Rendering.
		virtual void Render() override;
		virtual bool ShouldRender() const override { return true; }

	private:
		float2 m_TopPosition;
		float2 m_BottomPosition;
		Player* m_Player;
		uint32_t m_Height;
		float m_Speed;
		float m_LadderPositionPercentage;
		bool m_PlayerIsClimbing = false;
	};
}