#pragma once

namespace Pitfall
{
	class SpriteLibrary
	{
	public:
		static void AddSurface(Surface* surface, const String& key, uint32_t frameCount);
		static int IndexOf(const String& key);
		static Sprite* SpriteFromSurface(const String& key);

	private:
		inline static DynamicList<Surface*> m_Surfaces;
		inline static DynamicList<String> m_Keys;
		inline static DynamicList<uint32_t> m_FrameCounts;
	};
}