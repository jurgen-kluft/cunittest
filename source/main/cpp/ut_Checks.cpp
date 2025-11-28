#include "cunittest/private/ut_Checks.h"
#include "cunittest/private/ut_Utils.h"
#include "cunittest/private/ut_Config.h"

namespace UnitTest
{
    static void gCheckStringsEqual(TestResults& results, char const* const expected, char const* const actual, char const* const testName, char const* const filename, int const line, TestAllocator* allocator)
    {
        if (gAreStringsEqual(expected, actual) == false)
        {
            UnitTest::StringBuilder stringBuilder(allocator);
            stringBuilder << "Expected " << expected << " but was " << actual;
            results.onTestFailure(filename, line, testName, stringBuilder.getText());
        }
    }

    void checkEqual(TestResults& results, char const* expected, char const* actual, char const* testName, char const* filename, int line, TestAllocator* allocator)
    {
        gCheckStringsEqual(results, expected, actual, testName, filename, line, allocator);
    }

    void checkEqual(TestResults& results, char* expected, char* actual, char const* testName, char const* filename, int line, TestAllocator* allocator)
    {
        gCheckStringsEqual(results, expected, actual, testName, filename, line, allocator);
    }

    void checkEqual(TestResults& results, char* expected, char const* actual, char const* testName, char const* filename, int line, TestAllocator* allocator)
    {
        gCheckStringsEqual(results, expected, actual, testName, filename, line, allocator);
    }

    void checkEqual(TestResults& results, char const* expected, char* actual, char const* testName, char const* filename, int line, TestAllocator* allocator)
    {
        gCheckStringsEqual(results, expected, actual, testName, filename, line, allocator);
    }

} // namespace UnitTest
