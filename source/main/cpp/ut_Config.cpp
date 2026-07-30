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


    bool g_ShouldRunSuite(const char* test_filter, const char* suite_name)
    {
        if (test_filter == nullptr || test_filter[0] == '\0')
            return true;

        // test_filter example: "Suite1,Suite2/Fixture1,Suite3/Fixture2/Test1"
        // case sensitive
        // match the suite names, so terminate at the first '/', ',' or end-of-string.

        const int suite_name_len = gStringLength(suite_name);

        const char* iter = test_filter;
        while (*iter != '\0')
        {
            const char* start = iter;
            while (*iter != '\0' && *iter != '/' && *iter != ',')
                ++iter;

            const int len = (int)(iter - start);
            if (len == suite_name_len && gAreStringsEqual(start, suite_name))
                return true;

            if (*iter == ',')
                ++iter;
        }

        return false;
    }

    bool g_ShouldRunFixture(const char* test_filter, const char* fixture_name)
    {
        if (test_filter == nullptr || test_filter[0] == '\0')
            return true;

        // test_filter example: "Suite1,Suite2/Fixture1,Suite3/Fixture2/Test1"
        // case sensitive
        // match the fixture names, start at the first '/' if any, and terminate at the next '/' or ',' or end-of-string.
        // if testfilter doesn't specify any specific fixture, then all fixtures should be run.

        const int fixture_name_len = gStringLength(fixture_name);
        bool     hasFixtureFilter = false;

        const char* iter = test_filter;
        while (*iter != '\0')
        {
            while (*iter != '\0' && *iter != '/')
                ++iter;

            if (*iter == '/')
            {
                hasFixtureFilter = true;
                ++iter;
                const char* start = iter;
                while (*iter != '\0' && *iter != '/' && *iter != ',')
                    ++iter;

                const int len = (int)(iter - start);
                if (len == fixture_name_len && gAreStringsEqual(start, fixture_name))
                    return true;
            }

            if (*iter == ',')
                ++iter;
        }

        return !hasFixtureFilter; 
    }

    bool g_ShouldRunTest(const char* test_filter, const char* test_name)
    {
        if (test_filter == nullptr || test_filter[0] == '\0')
            return true;

        // test_filter example: "Suite1,Suite2/Fixture1,Suite3/Fixture2/Test1"
        // case sensitive
        // match the fixture names, start at the second '/' if any, and terminate at the next ',' or end-of-string.
        // if testfilter doesn't specify any specific test, then all tests should be run.

        const int test_name_len = gStringLength(test_name);
        
        bool     hasTestFilter = false;

        const char* iter = test_filter;
        while (*iter != '\0')
        {
            while (*iter != '\0' && *iter != '/')
                ++iter;

            if (*iter == '/')
            {
                ++iter;
                while (*iter != '\0' && *iter != '/')
                    ++iter;

                if (*iter == '/')
                {
                    hasTestFilter = true;

                    ++iter;
                    const char* start = iter;
                    while (*iter != '\0' && *iter != ',')
                        ++iter;

                    const int len = (int)(iter - start);
                    if (len == test_name_len && gAreStringsEqual(start, test_name))
                        return true;
                }
            }

            if (*iter == ',')
                ++iter;
        }

        return !hasTestFilter;
    }

} // namespace UnitTest
