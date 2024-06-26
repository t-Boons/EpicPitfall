#include "pfpch.h"

namespace Pitfall
{
	void AudioPlayer::SetAudioClip(const String& clip)
	{
		Assert(File::Exists(clip), (String("Sound file does not exist:") + clip).Cstr());

		m_Clip.loadSound(clip.Cstr());
	}

	void AudioPlayer::Play()
	{
		if (m_Clip.isPlaying())
		{
			m_Clip.replay();
		}
		else
		{
			m_Clip.play();
		}
	}

	void AudioPlayer::Pause()
	{
		m_Clip.pause();	
	}

	void AudioPlayer::Stop()
	{
		m_Clip.stop();
	}

	void AudioPlayer::SetVolume(float volume)
	{
		m_Clip.setVolume(volume);
	}

	void AudioPlayer::SetLooping(bool looping)
	{
		m_Clip.setLooping(looping);
	}
}