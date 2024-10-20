#ifdef TARGET_MAC

#include "cunittest/private/ut_File.h"

#include <stdio.h>
#include <cstdio>

namespace UnitTest
{
    void gFileWrite(const char* filename, void const* data, int size)
    {
        FILE* file = fopen(filename, "wb");
        if (file)
        {
            fwrite(data, 1, size, file);
            fclose(file);
        }
    }
}

#endif
