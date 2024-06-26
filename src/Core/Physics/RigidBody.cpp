#include "pfpch.h"

namespace Pitfall
{
	void RigidBody::BasicPhysicsUpdate(GameObject* object)
	{
		RigidBody* rb = object->GetRigidBody();

		if (!rb->m_EnablePhysics)
			return;

		float dt = Game::DeltaTime();

		rb->m_Velocity += rb->m_Gravity * dt;
		rb->m_Velocity = Math::Lerp(rb->m_Velocity, float2(0, 0), rb->m_Drag * rb->m_Mass * dt);

		object->SetPosition(object->GetPosition() + rb->m_Velocity * dt);
	}
}