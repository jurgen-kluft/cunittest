#include "cunittest/private/ut_Checks.h"
#include "cunittest/private/ut_Utils.h"
#include "cunittest/private/ut_ReportAssert.h"
#include "cunittest/private/ut_Config.h"

#include <stdlib.h>

namespace UnitTest
{
    const static int sPrefixSize  = 32;
    const static int sPostfixSize = 32;
    const static int sHeaderSize  = 16;

    void* MemCheckAllocator::Allocate(unsigned int size, unsigned int alignment)
    {
        if (alignment < sizeof(void*))
            alignment = sizeof(void*);

        const size_t totalSize = sPrefixSize + sHeaderSize + size + 2 * alignment + sPostfixSize;
        void*        ptr       = malloc(totalSize);

        // fill the whole memory with a 4 byte pattern
        const unsigned char pattern[]   = {0xCA, 0xFE, 0xBA, 0xBE};
        unsigned char*      fillptr     = (unsigned char*)ptr;
        unsigned char*      fillptr_end = fillptr + totalSize;
        while (fillptr < fillptr_end)
        {
            *fillptr = pattern[((size_t)fillptr) & 3];
            fillptr += 1;
        }

        // keep track of the memory we allocated so we can free it in Deallocate
        void* user_ptr          = (void*)(((size_t)ptr + sPrefixSize + sHeaderSize + alignment) & ~((size_t)alignment - 1));
        ((size_t*)user_ptr)[-3] = (size_t)ptr;
        ((size_t*)user_ptr)[-4] = size;
        return user_ptr;
    }

    void MemCheckAllocator::Deallocate(void* ptr, int* _status)
    {
        int    status = 0;
        size_t size   = 0;

        if (ptr)
        {
            const unsigned char pattern[] = {0xCA, 0xFE, 0xBA, 0xBE};

            // check the prefix and postfix for buffer overruns
            {
                unsigned char* prefix     = &((unsigned char*)ptr)[-16];
                unsigned char* end_prefix = &((unsigned char*)ptr)[0];
                while (prefix < end_prefix)
                {
                    if (*prefix != pattern[((size_t)prefix) & 3])
                    {
                        status = -1;
                        break;
                    }
                    prefix++;
                }
            }

            void* real_ptr = (void*)(((size_t*)ptr)[-3]); // get the pointer to the start of the allocation
            size           = ((size_t*)ptr)[-4];          // get the size of the allocation

            {
                unsigned char* end_prefix = (unsigned char*)&(((size_t*)ptr)[-4]);
                unsigned char* prefix     = (unsigned char*)real_ptr;
                while (prefix < end_prefix)
                {
                    if (*prefix != pattern[((size_t)prefix) & 3])
                    {
                        status = -1;
                        break;
                    }
                    prefix++;
                }
            }

            unsigned char* postfix     = &((unsigned char*)ptr)[size];
            unsigned char* postfix_end = &((unsigned char*)ptr)[size + sPostfixSize];
            while (postfix < postfix_end)
            {
                // check postfix
                if (*postfix != pattern[((size_t)postfix) & 3])
                {
                    status = 1;
                    break;
                }
                postfix++;
            }

            // free the memory
            free(real_ptr);
        }

        if (_status)
        {
            *_status = status;
        }
    }

} // namespace UnitTest
