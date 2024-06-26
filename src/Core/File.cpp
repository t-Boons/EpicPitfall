#include "pfpch.h"

namespace Pitfall
{
    File File::Open(const String& filePath)
    {
        // TODO Make this use the bitflags in the File properties (e.g. "r", "r+" etc.).
        // TODO Possibly make the file class less high level and add functions like Close(), Read() etc.
        // Functions to make it more like an API rather than a single function that reads the contents of a file and returns it.
        
        Assert(Exists(filePath), (String("File could not be found: ") + filePath).Cstr());


        FILE* cFile= fopen(filePath, "r");
        const uint32_t fileCharCount = Utils::GetFileCharCount(cFile);

        char* output = new char[fileCharCount + 1] {};

        fread(output, 1, fileCharCount, cFile);
        fclose(cFile);

        // Add null terminator;
        output[fileCharCount] = '\0';

        String fileContents(output);
        delete[] output;

        File file{};
        file.Contents = fileContents;
        file.FilePath = filePath;

        return file;
    }

    bool File::Exists(const String& filePath)
    {
        FILE* file = fopen(filePath, "r");

        if (file)
        {
            fclose(file);
        }

        return file;
    }

    void File::Write(const File& file)
    {
        FILE* cFile =   fopen(file.FilePath.Cstr(), "w+");

        Assert(cFile, "File does not exist.");

        fprintf(cFile, file.Contents.Cstr());

        fclose(cFile);
    }
}