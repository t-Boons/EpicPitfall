#include "pfpch.h"

namespace Pitfall
{
	template<typename InType>
	DynamicList<InType>::DynamicList(uint32_t initialSize)
		: m_Size(initialSize), m_Capacity(initialSize + 1), m_RefCount(new uint32_t(1))
	{
		m_ArrayPtr = new InType[initialSize + 1]{};
	}

	template<typename InType>
	DynamicList<InType>::DynamicList(InType* start, uint32_t size)
		: m_Size(size), m_Capacity(size + 1), m_RefCount(new uint32_t(1))
	{
		m_ArrayPtr = new InType[size + 1]{};

		for (size_t i = 0; i < size; i++)
		{
			m_ArrayPtr[i] = *(start + i);
		}
	}

	template<typename InType>
	DynamicList<InType>::~DynamicList()
	{
		if (--(*m_RefCount) == 0)
		{
			delete[] m_ArrayPtr;
			delete m_RefCount;
		}
	}

	template<typename InType>
	DynamicList<InType> DynamicList<InType>::operator=(const DynamicList<InType>& other)
	{
		if (&other == this)
		{
			return *this;
		}

		if (--(*m_RefCount) == 0)
		{
			delete m_RefCount;
		}

		m_RefCount = other.m_RefCount;
		(*m_RefCount)++;

		// Copy other contents of previous DynamicList.
		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;
		m_ArrayPtr = other.m_ArrayPtr;

		return *this;
	}

	template<typename InType>
	DynamicList<InType>::DynamicList(const DynamicList<InType>& list)
		: m_RefCount(list.m_RefCount), m_ArrayPtr(list.m_ArrayPtr), m_Size(list.m_Size), m_Capacity(list.m_Capacity)
	{
		(*m_RefCount)++;
	}

	template<typename InType>
	void DynamicList<InType>::Add(const InType& item)
	{
		m_Size++;
		if (m_Size > m_Capacity)
		{
			m_Capacity *= 2;
			InType* newArrayPtr = new InType[m_Capacity]{};

			// Copy array contents.
			for (uint32_t i = 0; i < m_Size - 1; i++)
			{
				newArrayPtr[i] = m_ArrayPtr[i];
			}

			delete[] m_ArrayPtr;

			m_ArrayPtr = newArrayPtr;
		}

		m_ArrayPtr[m_Size - 1] = item;
	}

	template<typename InType>
	void DynamicList<InType>::Remove(const InType& item)
	{
		for (uint32_t i = 0; i < m_Size; i++)
		{
			if (item == m_ArrayPtr[i])
			{
				RemoveAt(i);
			}
		}
	}

	template<typename InType>
	void DynamicList<InType>::RemoveAt(uint32_t item)
	{
		Assert(item < m_Size, "Index out of range");
		InType* newArrayPtr = new InType[m_Capacity]{};

		int i = 0;
		int j = 0;
		while (i < m_Size)
		{
			newArrayPtr[j] = m_ArrayPtr[i];

			j++;
			i++;

			if (i == item + 1)
			{
				j--;
			}

		}
		m_Size--;

		delete[] m_ArrayPtr;

		m_ArrayPtr = newArrayPtr;
	}

	template<typename InType>
	void DynamicList<InType>::SetSize(uint32_t size)
	{
		if (size > m_Capacity)
		{
			InType* newArrayPtr = new InType[size]{};

			// Copy array contents.
			for (uint32_t i = 0; i < min(size, m_Size); i++)
			{
				newArrayPtr[i] = m_ArrayPtr[i];
			}

			delete[] m_ArrayPtr;

			m_ArrayPtr = newArrayPtr;
		}

		m_Size = size;
	}
}
