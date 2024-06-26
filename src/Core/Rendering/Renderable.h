#pragma once

namespace Pitfall
{
	class Renderable
	{
	public:
		static void BasicRender(GameObject* object);
		static bool BasicShouldRender(const GameObject* object);

		~Renderable() { delete m_Sprite; }

	public:
		RectFloat m_SpriteRect = RectFloat({ {0, 0}, {1, 1} });	
		float2 m_SpriteOffset{};
		Sprite* m_Sprite = nullptr;
		int m_ZIndex = 0;
	};
}