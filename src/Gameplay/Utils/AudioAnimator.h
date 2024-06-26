#pragma once

/// The reason for using DyamicList instead of StaticList is that sounds are only added during object initialization. And having 4 big arrays that are mostly empty is a waste of memory.
/// Especially since the performance overhead of reallocating the dynamic arrays during initialization is not that severe.
/// The reason the audio players and bundles are seperated is to allow for multiple sounds per one audio name. They are seperated to avoid creating unneccesary bundles for singular sounds.

namespace Pitfall
{
	struct AudioBundle
	{
		DynamicList<AudioPlayer> AudioPlayers;
		String BundleName;
	};

	class AudioAnimator
	{
	public:
		void Tick();
		void AddSound(const String& name, const String& assetName);
		void AddSoundBundle(const String& name, const DynamicList<String>& assetNames);
		void PlaySoundOneShot(const String& name);
		void PlaySoundRepeating(const String& name, float delay);
		void Stop(const String& name);
		AudioPlayer& GetPlayer(const String name);
		int GetPlayerIndex(const String name);
		AudioBundle& GetBundle(const String name);
		int GetBundleIndex(const String name);

	private:
		DynamicList<String> m_AudioNames;

		DynamicList<AudioBundle> m_AudioBundles;
		DynamicList<float> m_AudioBundleDelays;
		DynamicList<Timer> m_AudioBundleDelayTimers;

		DynamicList<AudioPlayer> m_AudioPlayers;
		DynamicList<float> m_AudioPlayerDelays;
		DynamicList<Timer> m_AudioPlayerDelayTimers;
	};
}