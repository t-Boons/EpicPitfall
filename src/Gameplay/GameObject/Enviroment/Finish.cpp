#include "pfpch.h"

namespace Pitfall
{
	Finish::Finish(const String & name, const float2 & position)
		: GameObject(name)
	{
		SetPosition(position);

		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_IsTrigger = true;

		m_AABBCollider->m_AABB.SetSize({ 1.0f, 2.0f });

		m_Finished = false;
	}

	void Finish::OnCollision(const CollisionResult& result)
	{
		Player* player = TryCast<Player*>(result.Object);

		if (player && !m_Finished)
		{
			m_Finished = true;

			Print("You won C:");
			Game::SetActiveWorldAfterSeconds(new EndScreen(), FADE_SPEED);
		}
	}
}