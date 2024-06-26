#include "pfpch.h"

namespace Pitfall
{
	Zipline::Zipline(const String& name, const float2& begin, const float2& end, float speed)
		: GameObject(name)
	{
		SetPosition(begin);
		m_EndPoint = end;
		m_MovementSpeed = speed;

		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.SetSize(0.5f);
		m_AABBCollider->m_IsTrigger = true;
		m_AABBCollider->m_IsStatic = true;

		m_Renderable = new Renderable();

		m_DetachDirection = Math::Normalize(m_EndPoint - GetPosition());

		m_ZiplineSound.SetAudioClip("Assets/Audio/Zipline_Wind.wav");
		m_ZiplineSound.SetLooping(true);
	}

	void Zipline::Attach()
	{
		m_PlayerAttached = true;
		m_Player->GetAnimator().SetAnimation("Jump", false);
		m_Player->GetAnimator().SetSpeed(10.0f);
		m_Player->GetAnimator().SetAnimationDirection(m_EndPoint.x < GetPosition().x ? -1 : 1);
		m_Player->HandOverControls(true);
		m_ZiplineSound.Play();
	}

	void Zipline::Detach()
	{
		m_PlayerAttached = false;
		m_Player->GetRigidBody()->AddVelocity(m_DetachDirection * ZIPLINE_DETACHFORCE);
		m_Player->HandOverControls(false);
		m_ZiplineSound.Stop();
	}

	void Zipline::Start()
	{
		m_Player = GameObject::FindByType<Player>();
	}

	void Zipline::Tick(float deltaTime)
	{
		if (m_PlayerAttached)
		{
			m_TransitionProgress += deltaTime;
			m_Player->SetPosition(Math::LerpFloat2(GetPosition(), m_EndPoint, m_TransitionProgress * m_MovementSpeed) + m_AttachmentOffset);

			if (m_TransitionProgress >= 1)
			{
				Detach();
			}
		}
		else
		{
			m_TransitionProgress = 0;
		}

		m_ZiplineSound.SetVolume(0.1f + 0.9f * Math::Sin(m_TransitionProgress * Math::Pi));
	}

	void Zipline::Render()
	{
		const float2 startPoint = GetPosition();
		const float2 endPoint = m_EndPoint;

		const float iterations = Math::Length(startPoint - endPoint) * ZIPLINE_RENDER_ITERATIONS;

		for (float i = 0; i < iterations; i++)
		{
			const float2 point = Math::Lerp(startPoint, endPoint, i / iterations);
			Renderer::WorldDrawBoxSolid(point - float2(0.1f, 0.1f), point + float2(0.1f, 0.1f), RgbColor(43, 27, 4));
		}
	}

	bool Zipline::ShouldRender() const
	{
		return Renderer::WorldInFrame({ Math::Min(GetPosition().x, m_EndPoint.x), Math::Min(GetPosition().y, m_EndPoint.y) },
									  { Math::Max(GetPosition().x, m_EndPoint.x), Math::Max(GetPosition().y, m_EndPoint.y) });
	}
}