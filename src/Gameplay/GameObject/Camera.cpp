#include "pfpch.h"

namespace Pitfall
{
	Camera::Camera(const String& name)
		: GameObject(name), m_Pixelation(1), m_FadeBrightness(1), m_FadeDelta(0), m_LookDirection(0)
	{
	}

	void Camera::Start()
	{
		m_Background = GameObject::FindByType<ParralaxBackground>();
	}

	void Camera::Tick(float deltaTime)
	{
		if (m_ToFollow)
		{
			SetRenderSize(Math::Clamp(1.75f - Math::Length(m_ToFollow->GetPosition() + m_OffsetPosition - GetPosition()) * 0.05f, 1.0f, 2.0f));

			// Set camera position based on player position.
			m_OffsetPosition = float2(m_LookDirection * 2, 0);

			SetPosition(Math::Lerp(GetPosition(), m_ToFollow->GetPosition() + m_OffsetPosition, deltaTime * 6));
		}

		// If ParralaxBackground exists. Make it follow the camera.
		if (m_Background)
		{
			const float2 offsetPos = -GetPosition() * float2(10.0f, -10.0f) + float2(0, -500);
			m_Background->SetOffset(int2(Cast<int>(offsetPos.x), Cast<int>(offsetPos.y)));
		}


		// Slightly alter the camera's leaning position by pressing A and D.
		if (Game::IsKeyPressed(GLFW_KEY_A))
		{
			m_LookDirection = Math::Lerp(m_LookDirection, -1.5f, deltaTime * 4);
		}

		if (Game::IsKeyPressed(GLFW_KEY_D))
		{
			m_LookDirection = Math::Lerp(m_LookDirection, 1.5f, deltaTime * 4);
		}

		// Update fade delta.
		m_FadeBrightness = Math::Clamp(m_FadeBrightness + m_FadeDelta * deltaTime, 0.0f, 1.0f);
	}

	void Camera::ShaderStart()
	{
		// Runs when shader is initialized.
	}

	void Camera::ShaderTick()
	{
		// Runs every frame.

		Renderer::GetProperties().MainShader->SetFloat("time", Game::Time());
		Renderer::GetProperties().MainShader->SetFloat("brightness", m_FadeBrightness);

		// Pixelation effect.
		m_Pixelation += Game::DeltaTime() * 1.5f;
		m_Pixelation = m_Pixelation > 1 ? 1 : m_Pixelation;
		Renderer::GetProperties().MainShader->SetFloat("pixelationAmount", m_Pixelation);
	}
	void Camera::PlayPixelationEffect()
	{
		m_Pixelation = 0;
	}

	void Camera::FadeIn(float seconds)
	{
		m_FadeDelta = 1.0f / seconds;
		m_FadeBrightness = 0;
	}

	void Camera::FadeOut(float seconds)
	{
		m_FadeDelta = -1.0f / seconds;
	}
}