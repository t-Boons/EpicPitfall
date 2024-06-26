#include "pfpch.h"

namespace Pitfall
{
	namespace Utils
	{
		// Resource for figuring out how to convert a number to an integer array.
		// https://stackoverflow.com/questions/1860983/convert-integer-to-array
		DynamicList<int> NumberToIntArray(int number, uint32_t zeros)
		{
			uint32_t numberCount = IntegerNumberCount(number);

			const uint32_t size = Math::Max(numberCount, zeros);
			DynamicList<int> out(size);


			int i = size - 1;
			while(number >= 1)
			{
				out[i] = number % 10;
				i--;
				number /= 10;
			}

			return out;
		}
	}

	void NumberRenderer::Render()
	{
		for (uint32_t i = 0; i < m_Characters.Size(); i++)
		{
			m_Font->SetFrame(m_Characters[i]);
			Renderer::ScreenDrawSprite({m_Position + int2(m_Spacing * i, 0), m_Size}, m_Font);
		}
	}
	void NumberRenderer::SetNumber(int number)
	{
		m_Characters = Utils::NumberToIntArray(number, 5);
	}
}