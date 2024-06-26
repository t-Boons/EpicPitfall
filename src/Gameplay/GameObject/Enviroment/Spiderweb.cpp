#include "pfpch.h"

namespace Pitfall
{
	Spiderweb::Spiderweb(const String& name, const int2& position, float bounceStrength)
		: GameObject(name), m_BounceStrength(bounceStrength)
	{
		SetPosition(position - float2(0, 0.5f));

		m_Renderable = new Renderable();
		m_Renderable->m_SpriteRect.Size = float2(2, 2);
		m_Renderable->m_SpriteOffset = float2(0, 0.5);

		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.Size = float2(2.0f, 1.0f);
		m_AABBCollider->m_IsTrigger = true;

		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Bounce", SpriteLibrary::SpriteFromSurface("Spiderweb_Bounce"));
		m_Animator.AddAnimation("Idle", SpriteLibrary::SpriteFromSurface("Spiderweb_Idle"));

		DynamicList<String> bounceAssets(2);
		bounceAssets[0] = "Assets/Audio/Spiderweb/Spiderweb_Bounce0.wav";
		bounceAssets[1] = "Assets/Audio/Spiderweb/Spiderweb_Bounce1.wav";
		m_Audio.AddSoundBundle("Bounce", bounceAssets);
	}

	void Spiderweb::Tick(float)
	{
		m_Animator.SetAnimation("Idle");
		m_Animator.Tick();	
	}

	void Spiderweb::OnCollision(const CollisionResult& result)
	{
		if (result.Normal.y == -1)
		{
			Player* rb = TryCast<Player*>(result.Object);

			if (rb)
			{
				if (m_BounceResetTimer.elapsed() > 0.1f)
				{
					m_BounceResetTimer.reset();

					rb->Halt();
					rb->Jump(m_BounceStrength);
					m_Animator.SetAnimation("Bounce", false);
					m_Animator.SetSpeed(7.5f);
					m_Animator.OneShot();
					m_Audio.PlaySoundOneShot("Bounce");
				}
			}
		}
	}
}