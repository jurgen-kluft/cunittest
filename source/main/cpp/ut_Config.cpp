#include "cunittest/private/ut_Checks.h"
#include "cunittest/private/ut_Utils.h"
#include "cunittest/private/ut_ReportAssert.h"
#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_Test.h"

#include <stdlib.h>

namespace UnitTest
{
    struct StringSpan
    {
        const char* mStart;
        const char* mEnd;

        bool IsValid() const { return mStart != nullptr && mEnd != nullptr && mEnd > mStart; }
    };

    static bool s_HasTestFilter(const char* test_filter)
    {
        return test_filter != nullptr && test_filter[0] != '\0';
    }

    static bool s_AreNamesEqual(const StringSpan& span, const char* name)
    {
        if (!span.IsValid())
            return false;

        const int len = (int)(span.mEnd - span.mStart);
        return len == gStringLength(name) && gAreStringsEqualN(span.mStart, name, len);
    }

    static StringSpan s_ReadFilterToken(const char*& iter)
    {
        StringSpan span = {iter, iter};
        while (*iter != '\0' && *iter != '/' && *iter != ',')
            ++iter;
        span.mEnd = iter;
        return span;
    }

    static void s_SkipToNextFilterEntry(const char*& iter)
    {
        while (*iter != '\0' && *iter != ',')
            ++iter;

        if (*iter == ',')
            ++iter;
    }

    static bool s_ParseFilterEntry(const char*& iter, StringSpan& suiteSpan, StringSpan& fixtureSpan, StringSpan& testSpan)
    {
        suiteSpan.mStart   = nullptr;
        suiteSpan.mEnd     = nullptr;
        fixtureSpan.mStart = nullptr;
        fixtureSpan.mEnd   = nullptr;
        testSpan.mStart    = nullptr;
        testSpan.mEnd      = nullptr;

        suiteSpan = s_ReadFilterToken(iter);
        if (!suiteSpan.IsValid())
        {
            s_SkipToNextFilterEntry(iter);
            return false;
        }

        if (*iter == '/')
        {
            ++iter;
            fixtureSpan = s_ReadFilterToken(iter);
            if (!fixtureSpan.IsValid())
            {
                s_SkipToNextFilterEntry(iter);
                return false;
            }

            if (*iter == '/')
            {
                ++iter;
                testSpan = s_ReadFilterToken(iter);
                if (!testSpan.IsValid() || *iter == '/')
                {
                    s_SkipToNextFilterEntry(iter);
                    return false;
                }
            }
        }

        if (*iter != '\0' && *iter != ',')
        {
            s_SkipToNextFilterEntry(iter);
            return false;
        }

        if (*iter == ',')
            ++iter;

        return true;
    }

    static void s_EnableTest(TestFixture* fixture, Test* test)
    {
        if (!test->mRun)
        {
            test->mRun = true;
            ++fixture->mActiveTestCount;
        }
    }

    static void s_EnableFixture(TestSuite* suite, TestFixture* fixture)
    {
        if (!fixture->mRun)
        {
            fixture->mRun = true;
            ++suite->mActiveFixtureCount;
        }
    }

    static void s_EnableAllTests(TestFixture* fixture)
    {
        Test* test = fixture->mTestListHead;
        while (test != nullptr)
        {
            s_EnableTest(fixture, test);
            test       = test->mTestNext;
        }
    }

    static void s_EnableAllFixtures(TestSuite* suite)
    {
        TestFixture* fixture = suite->mFixtureListHead;
        while (fixture != nullptr)
        {
            s_EnableFixture(suite, fixture);
            s_EnableAllTests(fixture);
            fixture = fixture->mFixtureNext;
        }
    }

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

    void g_IterateFilter(const char* test_filter, TestSuite* suite_list)
    {
        if (!s_HasTestFilter(test_filter))
            return;

        const char* iter = test_filter;
        while (*iter != '\0')
        {
            StringSpan suiteSpan;
            StringSpan fixtureSpan;
            StringSpan testSpan;
            if (!s_ParseFilterEntry(iter, suiteSpan, fixtureSpan, testSpan))
            {
                continue;
            }

            TestSuite* suite = suite_list;
            while (suite != nullptr)
            {
                if (s_AreNamesEqual(suiteSpan, suite->mName))
                {
                    suite->mRun = true;

                    if (!fixtureSpan.IsValid())
                    {
                        s_EnableAllFixtures(suite);
                    }
                    else
                    {
                        TestFixture* fixture = suite->mFixtureListHead;
                        while (fixture != nullptr)
                        {
                            if (s_AreNamesEqual(fixtureSpan, fixture->mName))
                            {
                                s_EnableFixture(suite, fixture);

                                if (!testSpan.IsValid())
                                {
                                    s_EnableAllTests(fixture);
                                }
                                else
                                {
                                    Test* test = fixture->mTestListHead;
                                    while (test != nullptr)
                                    {
                                        if (s_AreNamesEqual(testSpan, test->mName))
                                            s_EnableTest(fixture, test);
                                        test = test->mTestNext;
                                    }
                                }
                            }
                            fixture = fixture->mFixtureNext;
                        }
                    }
                }
                suite = suite->mSuiteNext;
            }
        }
    }

} // namespace UnitTest
