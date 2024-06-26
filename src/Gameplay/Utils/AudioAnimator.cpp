#include "pfpch.h"

namespace Pitfall
{
	void AudioAnimator::Tick()
	{
		// Loop through players.
		for (uint32_t i = 0; i < m_AudioPlayers.Size(); i++)
		{
			if (m_AudioPlayerDelays[i] > 0 && m_AudioPlayerDelayTimers[i].elapsed() > m_AudioPlayerDelays[i])
			{
				m_AudioPlayerDelayTimers[i].reset();
				m_AudioPlayers[i].Play();
			}
		}

		// Loop through bundles.
		for (uint32_t i = 0; i < m_AudioBundleDelays.Size(); i++)
		{
			if (m_AudioBundleDelays[i] > 0 && m_AudioBundleDelayTimers[i].elapsed() > m_AudioBundleDelays[i])
			{
				m_AudioBundleDelayTimers[i].reset();
				const uint32_t size = m_AudioBundles[i].AudioPlayers.Size();
				m_AudioBundles[i].AudioPlayers[RandomUInt() % size].Play();
			}
		}
	}

	void AudioAnimator::AddSound(const String& name, const String& assetName)
	{
		m_AudioNames.Add(name);
		m_AudioPlayers.Add(AudioPlayer(assetName));
		m_AudioPlayerDelays.Add(-1.0f);
		m_AudioPlayerDelayTimers.Add(Timer());
	}

	void AudioAnimator::AddSoundBundle(const String& name, const DynamicList<String>& assetNames)
	{
		AudioBundle b;

		b.BundleName = name;

		b.AudioPlayers.SetSize(assetNames.Size());
		for (uint32_t i = 0; i < assetNames.Size(); i++)
		{
			b.AudioPlayers[i] = AudioPlayer(assetNames[i]);
		}

		m_AudioBundles.Add(b);
		m_AudioBundleDelays.Add(-1.0f);
		m_AudioBundleDelayTimers.Add(Timer());
	}

	void AudioAnimator::PlaySoundOneShot(const String& name)
	{
		// Try to play from player.
		int index = GetPlayerIndex(name);
		if (index >= 0)
		{
			m_AudioPlayers[index].Play();
			return;
		}

		// Try to play from bundle.
		index = GetBundleIndex(name);
		if (index >= 0)
		{
			const uint32_t size = m_AudioBundles[index].AudioPlayers.Size();
			m_AudioBundles[index].AudioPlayers[RandomUInt() % size].Play();
			return;
		}
	}

	void AudioAnimator::PlaySoundRepeating(const String& name, float delay)
	{
		// Try to play from player.
		int index = GetPlayerIndex(name);
		if (index >= 0)
		{
			m_AudioPlayerDelays[index] = delay;
			return;
		}

		// Try to play from bundle.
		index = GetBundleIndex(name);
		if (index >= 0)
		{
			m_AudioBundleDelays[index] = delay;
			return;
		}
	}

	void AudioAnimator::Stop(const String& name)
	{
		// Try to stop player.
		int index = GetPlayerIndex(name);
		if (index >= 0)
		{
			m_AudioPlayers[index].Stop();
			m_AudioPlayerDelays[index] = -1.0f;
			return;
		}

		// Try to stop bundle.
		index = GetBundleIndex(name);
		if (index >= 0)
		{
			const uint32_t size = m_AudioBundles[index].AudioPlayers.Size();
			for (uint32_t i = 0; i < size; i++)
			{
				m_AudioBundles[index].AudioPlayers[i].Stop();
			}

			m_AudioBundleDelays[index] = -1.0f;
			return;
		}
	}

	AudioPlayer& AudioAnimator::GetPlayer(const String name)
	{
		return m_AudioPlayers[GetPlayerIndex(name)];
	}

	int AudioAnimator::GetPlayerIndex(const String name)
	{
		for (uint32_t i = 0; i < m_AudioNames.Size(); i++)
		{
			if (m_AudioNames[i] == name)
			{
				return i;
			}
		}

		return -1;
	}

	AudioBundle& AudioAnimator::GetBundle(const String name)
	{
		return m_AudioBundles[GetPlayerIndex(name)];
	}

	int AudioAnimator::GetBundleIndex(const String name)
	{
		for (uint32_t i = 0; i < m_AudioBundles.Size(); i++)
		{
			if (m_AudioBundles[i].BundleName == name)
			{
				return i;
			}
		}

		return -1;
	}
}