#include "pfpch.h"

namespace Pitfall
{
	SwingingRope::SwingingRope(const String& name, const float2& pivotPos, float length)
		: GameObject(name)
	{
		SetPosition(pivotPos);

		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.SetSize(0.5f);
		m_AABBCollider->m_IsTrigger = true;

		m_Length = length;
		m_RenderIterations = Cast<uint32_t>(m_Length * 10);

		m_Renderable = new Renderable();

		m_SwingingSound.SetAudioClip("Assets/Audio/SwingingRope_Wind.wav");
		m_SwingingSound.SetLooping(true);
	}

	void SwingingRope::Attach()
	{
		if (m_AttachCooldown.elapsed() > 0.5f)
		{
			m_PlayerAttached = true;
			m_Player->GetAnimator().SetAnimation("Jump", false);
			m_Player->GetAnimator().SetSpeed(10.0f);
			m_Player->HandOverControls(true);

			m_SwingingSound.Play();
		}

		m_AttachCooldown.reset();
	}

	void SwingingRope::Detach()
	{
		m_AttachCooldown.reset();

		// Momentum calculation when detaching from the rope.
		const float2 movementDirection = Math::Normalize(m_EndPoint - m_OldEndPoint);
		m_Player->GetRigidBody()->AddVelocity(movementDirection * SWINGINGROPE_DETACHFORCE * (1.0f - Math::Abs(m_Delta)) * 0.5f + 0.5f);

		m_PlayerAttached = false;

		m_Player->HandOverControls(false);

		m_SwingingSound.Stop();
	}

	void SwingingRope::Start()
	{
		m_Player = GameObject::FindByType<Player>();
	}

	void SwingingRope::Tick(float)
	{
		// Temporary rotation resource for 2x2 rotation matrix.
		// https://www.google.com/search?sca_esv=566579981&sxsrf=AM9HkKl87YN6k1kZxjZnq7s9-u4AFPheDg:1695125715200&q=rotation+in+2x2+matrix&tbm=isch&source=lnms&sa=X&ved=2ahUKEwijo-qw07aBAxVw_7sIHXRsDPIQ0pQJegQICxAB&biw=2133&bih=1061&dpr=0.9
		// ChatGPT

		m_OldEndPoint = m_EndPoint;

		// Calculate rotation matrix for the endpoint vector. (Relative to the world position)
		m_Delta = Math::Sin(Game::Time() * SWINGINGROPE_SPEED * Math::Pi * 2);
		float clampedDelta = m_Delta * m_MaxAngle;

		m_RotationMatrix = { Math::Cos(clampedDelta), -Math::Sin(clampedDelta), Math::Sin(clampedDelta), Math::Cos(clampedDelta) };
		m_EndPoint = GetPosition() + m_RotationMatrix * float2(0, -m_Length);

		// Update player properties when attached.
		if (m_PlayerAttached)
		{
			m_Player->SetPosition(m_EndPoint + float2(0, -0.5f));

			const float2 movementDirection = Math::Normalize(m_EndPoint - m_OldEndPoint);
			m_Player->GetAnimator().SetAnimationDirection(movementDirection.x < 0 ? -1 : 1);

			m_SwingingSound.SetVolume(1.0f - Math::Abs(m_Delta));
		}
	}

	void SwingingRope::Render()
	{
		StaticList<float2, 3> points;
		points[0] = GetPosition();
		points[1] = Math::Lerp(GetPosition(), m_EndPoint, 0.33f) + float2(0, -m_Length * 0.25f);
		points[2] = m_EndPoint;

		const float2 startPoint = GetPosition();
		const float2 endPoint = m_EndPoint;

		for (float i = 0; i < m_RenderIterations; i++)
		{
			const float2 point = Math::BezierCurve(points, i / m_RenderIterations);
			Renderer::WorldDrawBoxSolid(point - float2(0.1f, 0.1f), point + float2(0.1f, 0.1f), RgbColor(112, 62, 12));
		}
	}


	bool SwingingRope::ShouldRender() const
	{
		const float2 bottomLeft = GetPosition() + float2(-m_Length, -m_Length);
		const float2 topRight = GetPosition() + float2(m_Length, 0);

		return Renderer::WorldInFrame(bottomLeft, topRight);
	}

	void SwingingRope::UpdateColliderPosition()
	{
		m_AABBCollider->m_AABB.Set(m_EndPoint);
	}
}