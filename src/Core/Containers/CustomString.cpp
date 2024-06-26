#include "pfpch.h"

namespace Pitfall
{
	namespace Utils
	{
		char* ConcatCStr(const char* A, const char* B)
		{
			size_t len1 = strlen(A);
			size_t len2 = strlen(B);

			char* out = new char [len1+ len2 + 1];
			memcpy(out, A, len1);
			memcpy(out + len1, B, len2 + 1);

			return out;
		}
	}

	String::String(Cstring str)
		: m_Size(0), m_RefCount(new uint32_t(1))
	{
		m_Size = Cast<uint32_t>(strlen(str));
		m_Cstr = new char[m_Size + 1];
		memcpy(m_Cstr, str, m_Size + 1);
	}

	String::String(Cstring str, uint32_t count)
		: m_Size(0), m_RefCount(new uint32_t(1))
	{
		Assert(strlen(str) > count, "Count is higher than string length");

		m_Size = count;
		m_Cstr = new char[m_Size + 1];
		memcpy(m_Cstr, str, m_Size + 1);
	}

	String::String(const String& other)
		: m_RefCount(other.m_RefCount)
	{
		(*m_RefCount)++;
		m_Size = other.m_Size;
		m_Cstr = other.m_Cstr;
	}

	String::~String()
	{
		if (--(*m_RefCount) == 0)
		{
			delete m_RefCount;
			delete[] m_Cstr;
		}
	}

	String String::Concat(const String& str) const
	{
		return Concat(str.Cstr());
	}

	String String::Concat(Cstring str) const
	{
		char* newCStr = Utils::ConcatCStr(m_Cstr, str);

		String out(newCStr);

		delete[] newCStr;
		return out;
	}

	String String::RemoveAll(char character) const
	{
		char* newStr = new char[m_Size];
		
		uint32_t j = 0;
		uint32_t i = 0;
		while(i < m_Size)
		{
			while(m_Cstr[i] == character)
			{
				i++;
			}

			newStr[j] = m_Cstr[i];

			i++;
			j++;
		}

		const int size = j + 1 * sizeof(char);
		char* outStr = new char[size];

		Assert(outStr, "Malloc returned null");

		memcpy(outStr, newStr, size - 1);
		outStr[j] = '\0';

		String out(outStr);

		delete[] outStr;
		delete[] newStr;
		return out;
	}

	String String::ReplaceAll(char check, char replacement) const
	{
		String out(m_Cstr);

		for (uint32_t i = 0; i < m_Size; i++)
		{
			out[i] = out[i] == check ? replacement : out[i];
		}

		return out;
	}

	String String::SubStr(uint32_t length, uint32_t begin) const
	{
		Assert(Cast<int>(begin + length ) - 1 < Cast<int>(m_Size), "Substr index out of range.");

		if (length == 0)
		{
			return String();
		}

		char* outPtr = new char[length * sizeof(char) + 1];
		Assert(outPtr, "Malloc returned nullptr")

		memcpy(outPtr, m_Cstr + begin, length * sizeof(char));
		outPtr[length] = '\0';

		String out((Cstring)outPtr);
		delete[]outPtr;

		return out;
	}

	bool String::Contains(const String& value) const
	{
		uint32_t valueLen = value.Len();
		
		uint32_t matchesFound = 0;
		uint32_t j = 0;
		for (uint32_t i = 0; i < m_Size; i++)
		{
			if (value[j] == m_Cstr[i])
			{
				j++;
				matchesFound++;
			}
			else
			{
				j = 0;
				matchesFound = 0;
			}

			if (matchesFound >= valueLen)
			{
				return true;
			}
		}

		return false;
	}

	int String::FindFirstIndexOf(char character) const
	{
		uint32_t i = 0;

		while (i < Len())
		{
			if (m_Cstr[i] == character)
			{
				break;
			}
			i++;
		}

		if (i == Len())
		{
			return -1;
		}

		return i;
	}

	int String::FindIndexOf(char character, uint32_t start) const
	{
		uint32_t i = Cast<int>(start);

		while (i < Len())
		{
			if (m_Cstr[i] == character)
			{
				break;
			}
			i++;
		}

		if (i == Len())
		{
			return -1;
		}

		return i;
	}

	int String::ContainsAmountOf(char character) const
	{
		int out = 0;
		for (size_t i = 0; i < m_Size; i++)
		{
			if (m_Cstr[i] == character)
			{
				out++;
			}
		}

		return out;
	}

	int String::ToInt(const String& str)
	{
		// Char values from 0 to 9 go from 48 to 57;
		// The - symbol is char code 45
		// https://www.learncpp.com/cpp-tutorial/chars/

		if (str.Len() <= 0)
		{
			return 0;
		}

		// See if value is negative or not.
		const bool isMinus = str.m_Cstr[0] == '-';

		int outValue = 0;
		int multiplier = 1;

		for (int i = Cast<int>(str.Len()) - 1; i >= Cast<int>(isMinus); i--)
		{
			bool containedNumber = false;

			// Loop through char codes
			for (char j = 0; j < 10; j++)
			{

				if (str.m_Cstr[i] == (j + 48))
				{
					outValue += j * multiplier;
					multiplier *= 10;
					containedNumber = true;
					break;
				}
			}

			if (!containedNumber)
			{
				return 0;
			}
		}

		return isMinus ? -outValue : outValue;
	}

	void String::operator=(Cstring str)
	{
		delete[] m_Cstr;
		m_Size = Cast<uint32_t>(strlen(str));
		m_Cstr = new char[m_Size + 1];
		memcpy(m_Cstr, str, m_Size + 1);
	}

	String String::ToString(int value)
	{
		const uint32_t size = Utils::IntegerNumberCount(value) + 1;
		char* buffer = new char[size];
		sprintf(buffer, "%i", value);
		buffer[size - 1] = '\0';
		String str = String(buffer);
		delete buffer;
		return str;
	}

	String String::operator=(const String& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (--(*m_RefCount) == 0)
		{
			delete m_RefCount;
		}

		m_RefCount = other.m_RefCount;
		(*m_RefCount)++;

		m_Size = other.m_Size;
		m_Cstr = other.m_Cstr;

		return *this;
	}

	char String::operator[](uint32_t index) const
	{
		Assert(index < m_Size, "Index out of range.")
		return m_Cstr[index];
	}

	char& String::operator [](uint32_t index)
	{
		Assert(index < m_Size, "Index out of range.")
		return m_Cstr[index];
	}

	String String::operator+(const String& str) const
	{
		String Out(m_Cstr);
		return Out.Concat(str);
	}

	String String::operator+(Cstring str) const
	{
		String Out(m_Cstr);
		return Out.Concat(str);
	}

	bool String::operator==(String str) const
	{
		return str == m_Cstr;
	}

	bool String::operator==(Cstring str) const
	{
		const uint32_t cstrLen = Cast<uint32_t>(strlen(str));
		if (cstrLen != Len())
		{
			return false;
		}

		for (uint32_t i = 0; i < cstrLen; i++)
		{
			if (str[i] != m_Cstr[i])
			{
				return false;
			}
		}

		return true;
	}
	bool String::operator!=(String str) const
	{
		return !(str == m_Cstr);
	}

	bool String::operator!=(Cstring str) const
	{
		const uint32_t cstrLen = Cast<uint32_t>(strlen(str));
		if (cstrLen != Len())
		{
			return true;
		}

		for (uint32_t i = 0; i < cstrLen; i++)
		{
			if (str[i] != m_Cstr[i])
			{
				return true;
			}
		}

		return false;
	}
}