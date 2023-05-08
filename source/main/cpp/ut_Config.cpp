#include "cunittest/private/ut_Checks.h"
#include "cunittest/private/ut_Utils.h"
#include "cunittest/private/ut_ReportAssert.h"
#include "cunittest/private/ut_Config.h"

#include <stdlib.h>

namespace UnitTest
{

    void* MemCheckAllocator::Allocate(unsigned int size, unsigned int alignment)
    {
        // we always want to have a prefix of 32 bytes and a postfix of 32 bytes to detect buffer overruns
        // we also need to adhere to the alignment requirements
        const int prefixSize  = 32;
        const int postfixSize = 32;
        const int headerSize  = 16;
        void*     ptr         = malloc(prefixSize + headerSize + size + alignment + postfixSize);
        void*     user_ptr    = (void*)(((size_t)ptr + alignment + prefixSize) & ~((size_t)alignment - 1));

        // fill the whole memory with 0xcd bytes
        unsigned char* p = (unsigned char*)ptr;
        for (size_t i = 0; i < prefixSize + headerSize + size + alignment + postfixSize; ++i)
        {
            *p++ = 0xcd;
        }

        // keep track of the memory we allocated so we can free it in Deallocate
        ((size_t*)user_ptr)[-3] = (size_t)ptr;
        ((size_t*)user_ptr)[-4] = size;

        return user_ptr;
    }

    unsigned int MemCheckAllocator::Deallocate(void* ptr, int* _status)
    {
        int    status = 0;
        size_t size   = 0;

        if (ptr)
        {
            // check the prefix and postfix for buffer overruns
            if (0xcdcdcdcdcdcdcdcdUL != ((size_t*)ptr)[-1])
                status = -1;
            if (0xcdcdcdcdcdcdcdcdUL != ((size_t*)ptr)[-2])
                status = -1;

            // get the pointer to the start of the allocation
            void* real_ptr = (void*)(((size_t*)ptr)[-3]);

            // get the size of the allocation
            size = ((size_t*)ptr)[-4];

            unsigned char* p = (unsigned char*)&(((size_t*)ptr)[-4]);
            for (size_t i = 0; i < 16; ++i)
            {
                // check prefix
                if (*--p != 0xcd)
                {
                    status = -1;
                    break;
                }
            }

            p = (unsigned char*)&(((unsigned char*)ptr)[size]);
            for (size_t i = 0; i < 32; ++i)
            {
                // check postfix
                if (*p++ != 0xcd)
                {
                    status = 1;
                    break;
                }
            }

            // free the memory
            free(real_ptr);
        }

        if (_status)
        {
            *_status = status;
        }

        return (unsigned int)size;
    }

} // namespace UnitTest
