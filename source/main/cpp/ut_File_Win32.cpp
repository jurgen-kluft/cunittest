#ifdef TARGET_PC

#include "cunittest/private/ut_File.h"

#include <stdio.h>
#include <cstdio>

namespace UnitTest
{
    void gFileWrite(const char* filename, void const* data, int size)
    {
        FILE* file = nullptr;
        fopen_s(&file, filename, "wb");
        if (file)
        {
            fwrite(data, 1, size, file);
            fclose(file);
        }
    }
}

#endif
