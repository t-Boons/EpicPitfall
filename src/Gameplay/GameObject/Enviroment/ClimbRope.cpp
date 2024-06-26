#include "pfpch.h"

namespace Pitfall
{
	ClimbRope::ClimbRope(const String& name, const int2& position, int height)
		: GameObject(name)
	{
		m_Height = height;
		m_Speed = CLIMBROPE_CLIMB_SPEED / Cast<float>(m_Height);

		SetPosition(position + float2(0.5f, m_Height * 0.5f));


		m_Renderable = new Renderable();
		m_Renderable->m_Sprite = SpriteLibrary::SpriteFromSurface("Vine");
		m_Renderable->m_SpriteRect.Size = { 1, Cast<float>(m_Height) };


		m_AABBCollider = new AABBCollider();
		m_AABBCollider->m_AABB.SetSize({1.0f, Cast<float>(m_Height)});
		m_AABBCollider->m_IsTrigger = true;

		// Set top and bottom position.
		m_BottomPosition = GetPosition() - float2(0, Cast<float>(m_Height) * 0.5f);
		m_TopPosition = GetPosition() + float2(0, Cast<float>(m_Height) * 0.5f);
	}

	void ClimbRope::Start()
	{
		m_Player = GameObject::FindByType<Player>();

		// Slightly adjust top and bottom position based on player's size.
		m_BottomPosition.y += m_Player->GetAABBCollider()->GetAABB().Size.y * 0.5f;
		m_TopPosition.y += m_Player->GetAABBCollider()->GetAABB().Size.y * 0.5f + 0.01f;
	}

	void ClimbRope::Tick(float)
	{
		bool playerOnTopOfLadder = PlayerOnTopOfLadder();
		m_AABBCollider->m_IsTrigger = !(playerOnTopOfLadder && !Game::IsKeyPressed(GLFW_KEY_S));

		if (m_PlayerIsClimbing)
		{
			m_Player->SetPosition(GetPositionFromPercentage(m_LadderPositionPercentage));
			m_Player->GetAnimator().SetAnimation("Climb");
			m_Player->HandOverControls(true);
			m_Player->EnablePhysics(false);	

			if (!m_AABBCollider->IsTrigger())
			{
				m_PlayerIsClimbing = false;
				m_Player->HandOverControls(false);
				m_Player->EnablePhysics(true);
			}
		}
	}


	void ClimbRope::OnCollision(const CollisionResult& collision)
	{
		Player* player = TryCast<Player*>(collision.Object);

		if (player)
		{
			if (m_PlayerIsClimbing)
			{
				m_Player->GetAnimator().SetSpeed(0);
			}
			else
			{
				m_LadderPositionPercentage = GetPercentageFromPosition(player->GetPosition());
			}

			// Enable climbing if player presses any of these buttons while colliding.
			if (Game::IsKeyDown(GLFW_KEY_W) || Game::IsKeyDown(GLFW_KEY_S))
			{
				m_PlayerIsClimbing = true;
			}

			// Climb up.
			if (Game::IsKeyPressed(GLFW_KEY_W))
			{
				m_LadderPositionPercentage += Game::DeltaTime() * m_Speed;
				m_Player->GetAnimator().SetSpeed(5);
			}

			// Climb down.
			if (Game::IsKeyPressed(GLFW_KEY_S))
			{
				m_LadderPositionPercentage -= Game::DeltaTime() * m_Speed;
				m_Player->GetAnimator().SetSpeed(5);

				if (m_LadderPositionPercentage < 0 && m_PlayerIsClimbing)
				{
					m_PlayerIsClimbing = false;
					m_Player->HandOverControls(false);
					m_Player->EnablePhysics(true);
				}
			}

			// Eject player.
			if (Game::IsKeyDown(GLFW_KEY_SPACE) && m_PlayerIsClimbing)
			{
				m_Player->HandOverControls(false);
				m_Player->EnablePhysics(true);
				m_Player->Jump(m_Player->GetJumpForce());
				m_LadderPositionPercentage = 0;
				m_PlayerIsClimbing = false;
			}

			// Clamp ladder position so that the player can not climb too low or too high.
			m_LadderPositionPercentage = Math::Clamp(m_LadderPositionPercentage, 0.0f, 1.0f);
		}
	}

	bool ClimbRope::PlayerOnTopOfLadder() const
	{
		 return m_Player->GetPosition().y > m_TopPosition.y - 0.05f &&
				m_Player->GetPosition().y < m_TopPosition.y + 0.1f &&
				m_Player->GetPosition().x < m_TopPosition.x + 0.5f &&
				m_Player->GetPosition().x > m_TopPosition.x - 0.5f;
	}

	void ClimbRope::Render()
	{
		// Render the vine.
		const float2 endPosition = GetPosition() + float2(0, -Cast<float>(m_Height * 0.5f) + 0.5f);
		m_Renderable->m_Sprite->SetFrame(1);
		Renderer::WorldDrawSprite({endPosition, { 1.0f, 1.0f } }, m_Renderable->m_Sprite);

		m_Renderable->m_Sprite->SetFrame(0);
		for (uint32_t i = 1; i < m_Height; i++)
		{
			Renderer::WorldDrawSprite({ endPosition + float2(0, Cast<float>(i)), {1.0f, 1.0f}}, m_Renderable->m_Sprite);
		}
	}
}