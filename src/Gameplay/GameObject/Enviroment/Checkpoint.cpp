#include "pfpch.h"

namespace Pitfall
{
	Checkpoint::Checkpoint(const String& name, const float2& position, int index)
		: GameObject(name), m_Collected(false), m_Index(index)
	{
		SetPosition(position);

		m_Renderable = new Renderable();
		m_Renderable->m_SpriteRect.Size = { 1, 2 };
		m_Renderable->m_ZIndex = 3;

		m_Animator.SetSubject(m_Renderable);
		m_Animator.AddAnimation("Transition", SpriteLibrary::SpriteFromSurface("Checkpoint"));

		m_PixelCollider = new PixelCollider();
		m_PixelCollider->Init(m_Renderable, Utils::RectToAABB(m_Renderable->m_SpriteRect));

		m_Sound.SetAudioClip("Assets/Audio/Checkpoint_Chime.wav");
	}

	void Checkpoint::Start()
	{
		m_Player = FindByType<Player>();
		m_Animator.SetAnimation("Transition", false);
		m_Animator.SetSpeed(0);

		if (File::Exists(CHECKPOINT_FILEPATH))
		{
			int index = String::ToInt(File::Open(CHECKPOINT_FILEPATH).Contents);

			if (index == m_Index)
			{
				m_Player->SetRespawnPosition(GetPosition() + float2(0, 1));
				m_Player->SetPosition(GetPosition());
			}
			else if (index >= m_Index)
			{
				Collect();
			}
		}
	}

	void Checkpoint::Tick(float)
	{
		m_Animator.Tick();
	}

	void Checkpoint::OnCollision(const CollisionResult& result)
	{
		if (result.CollisionType == CollisionType::Pixel)
		{
			if (!m_Collected)
			{
				if (m_Player == result.Object)
				{
					Collect();
					Save();
				}
			}
		}
	}
	void Checkpoint::Collect()
	{
		m_Animator.SetSpeed(8);
		m_Collected = true;
	}
	void Checkpoint::Save()
	{
		bool fExists = File::Exists(CHECKPOINT_FILEPATH);

		bool indexIsHigher = false;
		if (fExists)
		{
			const int index = String::ToInt(File::Open(CHECKPOINT_FILEPATH).Contents);
			indexIsHigher = index <= m_Index;
		}

		if (!fExists || indexIsHigher)
		{
			File::Write({ CHECKPOINT_FILEPATH, String::ToString(m_Index) });

			m_Sound.Play();
			m_Player->SetRespawnPosition(GetPosition());
		}
	}
}