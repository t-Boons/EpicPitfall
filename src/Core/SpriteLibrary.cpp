#include "pfpch.h"

namespace Pitfall
{
	void SpriteLibrary::AddSurface(Surface* surface, const String& key, uint32_t frameCount) // Big: O(1)
	{
		m_Surfaces.Add(surface);
		m_Keys.Add(key);
		m_FrameCounts.Add(frameCount);
	}

	int SpriteLibrary::IndexOf(const String& key) // Big: O(n)
	{
		// TODO: Make this in to a hashmap so its faster.
		for (uint32_t i = 0; i < m_Surfaces.Size(); i++)
		{
			if (m_Keys[i] == key)
			{
				return i;
			}
		}

		Print(key.Cstr());
		Assert(false, "Sprite does not exist in library.");
		return -1;
	}

	Sprite* SpriteLibrary::SpriteFromSurface(const String& key) // Big: O(1)
	{
		const int index = IndexOf(key);
		if (index >= 0)
		{
			return new Sprite(m_Surfaces[index], m_FrameCounts[index]);
		}

		return nullptr;
	}
}