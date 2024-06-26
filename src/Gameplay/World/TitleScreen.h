#pragma once

namespace Pitfall
{
	class Player;

	class TitleScreen : public World
	{
	public:
		virtual void Load() override;
		virtual void Tick(float deltaTime) override;
		virtual void Unload() override;

	private:
		Player* m_Player;
		AudioPlayer m_Music;
	};
}