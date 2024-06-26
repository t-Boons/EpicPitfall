#pragma once

// Resource for finding how to read and write to files.
// https://www.freecodecamp.org/news/file-handling-in-c-how-to-open-close-and-write-to-files/

// Resource for learning how to use bitflags
// https://dietertack.medium.com/using-bit-flags-in-c-d39ec6e30f08

namespace Pitfall
{
	struct File
	{
	public:
		File() {}
		File(const String& filePath, const String& contents)
			: FilePath(filePath), Contents(contents)
		{}

		static File Open(const String& filePath);
		static bool Exists(const String& filePath);
		static void Write(const File& file);

	public:
		String FilePath;
		String Contents;
	};
}