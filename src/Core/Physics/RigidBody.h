#pragma once

namespace Pitfall
{
	class RigidBody
	{
	public:
		static void BasicPhysicsUpdate(GameObject* object);

	public:
		const float2& GetVelocity() const { return m_Velocity; }
		void AddVelocity(const float2& velocity) { m_Velocity += velocity; }
		void SetVelocity(const float2& velocity) { m_Velocity = velocity; }


	public:
		float2 m_Gravity = float2(0, -9.0f);
		float2 m_Velocity{};
		float m_Drag = 0;
		float m_Mass = 1;
		float m_Friction = 1;
		bool m_EnablePhysics = true;
	};
}