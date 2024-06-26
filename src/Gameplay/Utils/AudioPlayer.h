#pragma once

namespace Pitfall
{
	class AudioPlayer
	{
	public:
		AudioPlayer() {}
		AudioPlayer(const String& assetName)
		{
			SetAudioClip(assetName);
		}

		void SetAudioClip(const String& clip);

		void Play();
		void Pause();
		void Stop();
		void SetVolume(float volume);
		void SetLooping(bool looping);

	private:
		Audio::Sound m_Clip;
	};
}