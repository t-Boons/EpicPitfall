#pragma once

namespace Pitfall
{
	template<typename InType>
	class DynamicList
	{
	public:
		DynamicList(uint32_t initialSize = 0);
		DynamicList(InType* start, uint32_t size);

		~DynamicList();

		DynamicList(const DynamicList<InType>& list);

		/// <summary>
		/// Returns the size of the List.
		/// </summary>
		/// <returns></returns>
		uint32_t Size() const { return m_Size; }

		/// <summary>
		/// Adds an item to the List.
		/// </summary>
		/// <param name="Item"></param>
		void Add(const InType& item);

		/// <summary>
		/// Removes an item from the List.
		/// </summary>
		/// <param name="Item"></param>
		void Remove(const InType& item);

		/// <summary>
		/// Removes an item from the list based on Index.
		/// </summary>
		/// <param name="Index"></param>
		void RemoveAt(uint32_t index);

		/// <summary>
		/// Set the size of the List.
		/// </summary>
		/// <param name="Size"></param>
		void SetSize(uint32_t size);

		InType* begin() const { return &m_ArrayPtr[0]; }
		InType* end() const { return &m_ArrayPtr[m_Size]; }

		InType& operator[](uint32_t index)
		{
			Assert(index < m_Size, "Index out of range");
			return m_ArrayPtr[index];
		}

		const InType& operator[](uint32_t index) const
		{
			Assert(index < m_Size, "Index out of range");
			return m_ArrayPtr[index];
		}

		DynamicList<InType> operator =(const DynamicList<InType>& list);

	private:
		InType* m_ArrayPtr;
		uint32_t* m_RefCount;
		uint32_t m_Size;
		uint32_t m_Capacity;
	};
}

// Resource:
// https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp

// Excluded DynamicList.cpp from project.
// Required to make templates work in cpp file.
#include "DynamicList.cpp"