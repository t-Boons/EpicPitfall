
// Resource because I had some trouble figuring out how to concatinate c-style strings.
// I ended up using memcopy instead as described on the webpage.
// https://stackoverflow.com/questions/5614411/correct-way-to-malloc-space-for-a-string-and-then-insert-characters-into-that-sp 

// Reference counting concept and related code examples inspired and partially taken from ChatGPT.

#pragma once

namespace Pitfall
{
	class String
	{
	public:
		typedef const char* Cstring;

		String(Cstring str = "");
		String(Cstring str, uint32_t count);
		String(const String& other);

		~String();

		char begin() const { return m_Cstr[0]; }
		char end() const { return m_Cstr[m_Size]; }

		/// <summary>
		/// Adds another string to this one.
		/// </summary>
		/// <param name="Str"></param>
		String Concat(const String& str) const;

		/// <summary>
		/// Adds another string to this one.
		/// </summary>
		/// <param name="Str"></param>
		String Concat(Cstring str) const;

		/// <summary>
		/// Remove all chars from string.
		/// </summary>
		/// <param name="Character"></param>
		String RemoveAll(char character) const;

		/// <summary>
		/// Replace all chars from string if it matches the replacement.
		/// </summary>
		/// <param name="Character"></param>
		String ReplaceAll(char check, char replacement) const;

		/// <summary>
		/// Create a new string from within this one.
		/// </summary>
		/// <param name="Length"></param>
		/// <param name="Begin"></param>
		/// <returns></returns>
		String SubStr(uint32_t length, uint32_t begin) const;

		/// <summary>
		/// Get C-string.
		/// </summary>
		/// <returns></returns>
		Cstring Cstr() const { return m_Cstr; }

		/// <summary>
		/// Get the amount of characters in the string.
		/// </summary>
		/// <returns></returns>
		uint32_t Len() const { return m_Size; }


		/// <summary>
		/// Check if string contains text.
		/// </summary>
		/// <param name="Value"></param>
		/// <returns></returns>
		bool Contains(const String& value) const;

		/// <summary>
		/// Returns the first index of the Character occuring.
		/// </summary>
		/// <param name="Character"></param>
		/// <returns></returns>
		int FindFirstIndexOf(char character) const;

		/// <summary>
		/// Returns the first index of the Character occuring.
		/// </summary>
		/// <param name="Character"></param>
		/// <returns></returns>
		int FindIndexOf(char character, uint32_t start) const;

		/// <summary>
		/// Returns the amount of times said Character gets found in the string.
		/// </summary>
		/// <param name="Character"></param>
		/// <returns></returns>
		int ContainsAmountOf(char character) const;

		/// <summary>
		/// Parses a string to an int;
		/// </summary>
		/// <param name="Str"></param>
		/// <returns></returns>
		static int ToInt(const String& str);

		/// <summary>
		/// Convert integer to a string.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static String ToString(int value);

		String operator =(const String& Other);
		void operator =(Cstring Str);
		String operator +=(Cstring str) { return(Concat(str)); }
		String operator +=(const String& str) { return(Concat(str)); }
		char operator [](uint32_t index) const;
		char& operator [](uint32_t index);
		String operator +(const String& str) const;
		String operator +(Cstring str) const;
		bool operator ==(String str) const;
		bool operator ==(Cstring str) const;

		bool operator !=(String str) const;
		bool operator !=(Cstring str) const;
		operator Cstring () const { return m_Cstr; }

	private:
		uint32_t m_Size;
		char* m_Cstr;
		uint32_t* m_RefCount;
	};
}