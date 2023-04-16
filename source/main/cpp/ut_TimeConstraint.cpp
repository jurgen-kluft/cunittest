#include "cunittest/private/ut_TimeConstraint.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_StringBuilder.h"
#include "cunittest/private/ut_TimeHelpers.h"

namespace UnitTest
{
    TimeConstraint::TimeConstraint(int ms, TestResults& result, char const* filename, int lineNumber, char const* testName, TestAllocator* allocator)
        : mResult(result)
        , mMaxMs(ms)
        , mFilename(filename)
        , mLineNumber(lineNumber)
        , mTestName(testName)
        , mAllocator(allocator)
    {
        mStartTime = g_TimeStart();
    }

    TimeConstraint::~TimeConstraint()
    {
        const int totalTimeInMs = (int)(g_GetElapsedTimeInMs(mStartTime));
        if (totalTimeInMs > mMaxMs)
        {
            StringBuilder strBuilder(mAllocator);
            strBuilder << "Time constraint failed. Expected to run test under " << mMaxMs << "ms but took " << totalTimeInMs << "ms.";
            mResult.onTestFailure(mFilename, mLineNumber, mTestName, strBuilder.getText());
        }
    }
} // namespace UnitTest
