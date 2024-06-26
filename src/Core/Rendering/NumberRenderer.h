#pragma once

namespace Pitfall
{
	class  NumberRenderer
	{
	public:
		void Render();
		void SetNumber(int number);
		void SetNumberFont(Sprite* fontSprite) { m_Font = fontSprite; }
		void SetPosition(const int2& position) { m_Position = position; }
		void SetSize(const int2& size) { m_Size = size; }
		void SetSpacing(int spacing) { m_Spacing = spacing; }

	private:
		int2 m_Size;
		int2 m_Position;
		Sprite* m_Font;
		int m_Number;
		int m_Spacing;
		DynamicList<int> m_Characters;
	};
}