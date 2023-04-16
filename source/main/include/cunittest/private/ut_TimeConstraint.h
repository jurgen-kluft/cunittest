#ifndef __C_CORE_UNITTEST_TIMECONSTRAINT_H__
#define __C_CORE_UNITTEST_TIMECONSTRAINT_H__

namespace UnitTest
{
    class TestResults;
    class TestAllocator;

    class TimeConstraint
    {
    public:
        TimeConstraint(int ms, TestResults& result, char const* filename, int lineNumber, char const* testName, TestAllocator* allocator);
        ~TimeConstraint();

    private:
        void operator=(const TimeConstraint&);

        TestResults&      mResult;
        int const         mMaxMs;
        char const* const mFilename;
        int const         mLineNumber;
        char const* const mTestName;
        unsigned int      mStartTime;
        TestAllocator*    mAllocator;
    };

#define UNITTEST_TIME_CONSTRAINT(ms) TimeConstraint t__(ms, testResults_, __FILE__, __LINE__, testName, Allocator)
#define UNITTEST_TIME_CONSTRAINT_EXEMPT() mTimeConstraintExempt = true
} // namespace UnitTest

#endif /// __C_CORE_UNITTEST_TIMECONSTRAINT_H__
