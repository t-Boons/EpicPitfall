#include "pfpch.h"

namespace Pitfall
{
	namespace Utils
	{
		void SetDirection(Renderable* renderable, int direction)
		{
			if (renderable->m_SpriteRect.Size.x > 0 && direction == -1)
			{
				renderable->m_SpriteRect.Size.x = -renderable->m_SpriteRect.Size.x;
				return;
			}

			if (renderable->m_SpriteRect.Size.x < 0 && direction == 1)
			{
				renderable->m_SpriteRect.Size.x = -renderable->m_SpriteRect.Size.x;
				return;
			}
		}
	}

	void Animator::SetSubject(Renderable* animRenderer)
	{
		m_AnimationRenderer = animRenderer;
	}

	void Animator::Tick()
	{
		Assert(m_AnimationRenderer->m_Sprite, "No animation found.");

		if (m_AnimationRenderer->m_Sprite && m_AnimTimer.elapsed() > 1.0f / m_Speed)
		{
			m_AnimTimer.reset();
			m_FrameIndex++;

			if (m_Looping)
			{
				m_FrameIndex %= m_AnimationRenderer->m_Sprite->Frames();
			}
			else
			{
				m_FrameIndex = m_FrameIndex < m_AnimationRenderer->m_Sprite->Frames() ? m_FrameIndex : m_AnimationRenderer->m_Sprite->Frames() - 1;
			}

			if (m_FrameIndex == m_AnimationRenderer->m_Sprite->Frames() - 1)
			{
				m_OneShot = false;
			}

			m_AnimationRenderer->m_Sprite->SetFrame(m_FrameIndex);
		}
	}

	void Animator::AddAnimation(const String& name, Sprite* sprite)
	{
		m_AnimationNames.Add(name);
		m_AnimationSprites.Add(sprite);

		if (!m_AnimationRenderer->m_Sprite)
		{
			SetAnimation(name);
		}
	}

	void Animator::SetAnimation(const String& name, bool looping)
	{
		if (m_OneShot)
		{
			return;
		}

		if (m_ActiveAnimation == name)
		{
			m_Looping = looping;
			return;
		}

		m_FrameIndex = 0;
		m_AnimTimer.reset();

		for (uint32_t i = 0; i < m_AnimationNames.Size(); i++)
		{
			if (m_AnimationNames[i] == name)
			{
				m_Looping = looping;
				m_ActiveAnimation = name;
				m_AnimationRenderer->m_Sprite = m_AnimationSprites[i];
				return;
			}
		}

		Assert(false, (String("Animation name is invalid: ") + name).Cstr());
	}

	void Animator::SetSpeed(float speed)
	{
		if (m_OneShot)
			return;

		m_Speed = speed;
	}

	void Animator::SetAnimationDirection(int direction)
	{
		if (m_OneShot)
			return;

		Utils::SetDirection(m_AnimationRenderer, direction);
	}
}