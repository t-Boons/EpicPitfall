#include "pfpch.h"

namespace Pitfall
{
	Coin::Coin(const String& name, const float2& position)
		: GameObject(name), m_ShouldDestroy(false)
	{
		SetPosition(position);

		m_Renderable = new Renderable();
		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Rotate", SpriteLibrary::SpriteFromSurface("Coin"));

		m_PixelCollider = new PixelCollider();
		m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));

		m_Sound.SetAudioClip("Assets/Audio/Coin_Pickup.wav");
	}

	void Coin::Start()
	{
		m_Animator.SetAnimation("Rotate");
		m_Animator.SetSpeed(5.0f);
	}

	void Coin::Tick(float)
	{
		m_Animator.Tick();

		if (!m_ShouldDestroy)
		{
			m_DestroyTimer.reset();
		}

		if (m_DestroyTimer.elapsed() > 0.15f)
		{
			Destroy(this);
		}
	}

	void Coin::OnCollision(const CollisionResult& result)
	{
		if (!m_ShouldDestroy)
		{
			Player* player = TryCast<Player*>(result.Object);
			if (player)
			{
				player->AddScore(COIN_SCORE);
				m_Sound.Play();
				m_ShouldDestroy = true;
			}
		}
	}
}