#pragma once

namespace Pitfall
{
	template<typename InType, uint32_t InSize>
	class StaticList
	{
	public:

		/// <summary>
		/// Returns the size of the List.
		/// </summary>
		/// <returns></returns>
		inline uint32_t Size() const { return InSize; }

		const InType* begin() const { return &m_Array[0]; }
		const InType* end() const { return &m_Array[InSize]; }
		InType* begin() { return &m_Array[0]; }
		InType* end() { return &m_Array[InSize]; }

		InType& operator[](uint32_t index)
		{
			Assert(index < InSize, "Index out of range");
			return m_Array[index];
		}

	private:
		InType m_Array[InSize]{};
	};
}