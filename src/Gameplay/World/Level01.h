#pragma once

namespace Pitfall
{
	class Level01 : public World
	{
	public:
		virtual void Load() override;
		virtual void Unload() override;
	private:
		AudioPlayer m_Music;
	};
}