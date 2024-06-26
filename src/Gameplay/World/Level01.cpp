#include "pfpch.h"

namespace Pitfall
{
	void Level01::Load()
	{
		// gList is a dynamic list because it is only updated and resized while loading the level.
		// This makes the design way easier without effecting framerate during runtime.
		DynamicList<GameObject*> glist = DataSerializer::MapFromJson("Assets/Maps/Level01.json");

		m_Objects[0] = new Camera("Camera");
		m_Objects[1] = new ParralaxBackground("ParralaxBackgroundLayer1");
		m_Objects[2] = new NumberDisplay("ScoreDisplay", Renderer::ScreenPercentage({ 0.1f, 0.1f }));
		m_Objects[3] = new NumberDisplay("HealthDisplay", Renderer::ScreenPercentage({ 0.8f, 0.1f }));

		for (uint32_t i = 0; i < glist.Size(); i++)
		{
			m_Objects[i + 4] = glist[i];
		}

		m_Music.SetAudioClip("Assets/Audio/Music/Music_Level01.wav");
		m_Music.SetVolume(0.6f);
		m_Music.SetLooping(true);
		m_Music.Play();


		World::Load();

		// Make the camera follow the player.
		GameObject::FindByType<Camera>()->ObjectToFollow(GameObject::FindByType<Player>());
	}

	void Level01::Unload()
	{
		m_Music.Stop();
		World::Unload();
	}
}