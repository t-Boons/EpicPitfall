#include "pfpch.h"

namespace Pitfall
{
	void TitleScreen::Load()
	{
		m_Objects[0] = new Camera("Camera");;
		m_Objects[1] = new StartButton("StartButton", { Renderer::ScreenPercentage({0.5f, 0.75f}), Renderer::ScreenPercentage({0.2f, 0.1f}) });
		m_Objects[2] = new Image("Background_TitleScreen", { Renderer::ScreenPercentage({0.5f, 0.5f}), Renderer::ScreenSize()}, SpriteLibrary::SpriteFromSurface("Background_TitleScreen"));
		m_Player = new Player("AnimatedPlayer", { -15, 1 });
		m_Objects[3] = m_Player;
		m_Objects[4] = new BoxCollider("PlayerPlatform", RectFloat({0, -0.5f}, {40, 1}));

		// Play titlescreen audio.
		m_Music.SetAudioClip("Assets/Audio/Music/Music_MainMenu.wav");
		m_Music.SetLooping(true);
		m_Music.Play();

		World::Load();
	}

	void TitleScreen::Tick(float deltaTime)
	{
		// Make sure the player loops around the screen.
		if (m_Player)
		{
			if (m_Player->GetPosition().x > 20.0f)
			{
				m_Player->SetPosition({ -20, m_Player->GetPosition().y });
			}
			else if (m_Player->GetPosition().x < -20.0f)
			{
				m_Player->SetPosition({ 20, m_Player->GetPosition().y });
			}
		}

		World::Tick(deltaTime);
	}

	void TitleScreen::Unload()
	{
		m_Music.Stop();
		World::Unload();
	}
}
