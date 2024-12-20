#ifndef __C_CORE_UNITTEST_TIMECONSTRAINT_H__
#define __C_CORE_UNITTEST_TIMECONSTRAINT_H__

#include "cunittest/private/ut_TimeHelpers.h"
#include "cunittest/private/ut_TestResults.h"

namespace UnitTest
{
    class TestAllocator;

    class TimeConstraint
    {
    public:
        TimeConstraint(int ms, TestResults& result, char const* filename, int lineNumber, char const* testName, TestAllocator* allocator);
        ~TimeConstraint();

        TestResults&      mResult;
        int const         mMaxMs;
        char const* const mFilename;
        int const         mLineNumber;
        char const* const mTestName;
        time_t            mStartTime;
        TestAllocator*    mAllocator;
    };

#define UNITTEST_TIME_CONSTRAINT(ms, allocator) TimeConstraint t__(ms, testResults_, __FILE__, __LINE__, testName, allocator)
#define UNITTEST_TIME_CONSTRAINT_EXEMPT() mTimeConstraintExempt = true
} // namespace UnitTest

#endif /// __C_CORE_UNITTEST_TIMECONSTRAINT_H__
