#pragma once

namespace Pitfall
{
	class SwingingRope : public GameObject
	{
	public:
		SwingingRope(const String& name, const float2& pivotPos, float length);

		// SwingingRope Specific.
		void Attach();
		bool Attached() const { return m_PlayerAttached; }
		void Detach();

		// GameObject.
		virtual void Start() override;
		virtual void Tick(float deltaTime) override;

		// Rendering.
		virtual void Render() override;
		virtual bool ShouldRender() const override;

		// Collision.
		virtual void UpdateColliderPosition() override;

	private:
		mat2 m_RotationMatrix;
		float2 m_OldEndPoint;
		float2 m_EndPoint;
		Player* m_Player;
		float m_Length;
		uint32_t m_RenderIterations;

		AudioPlayer m_SwingingSound;

		const float m_MaxAngle = SWINGINGROPE_MAXANGLE * Math::Deg2Rad;

		// Dynamic.
		float m_Delta;
		bool m_PlayerAttached = false;
		Timer m_AttachCooldown;

	};
}