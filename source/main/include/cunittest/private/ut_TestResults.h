#ifndef __CUNITTEST_TESTRESULTS_H__
#define __CUNITTEST_TESTRESULTS_H__

namespace UnitTest
{
    class TestReporter;

    class TestResults
    {
    public:
        explicit TestResults(TestReporter* reporter = 0);

        void onTestSuiteStart(char const* testName, int inNumTests);
        void onTestSuiteEnd(char const* testName, float secondsElapsed);
        void onTestFixtureStart(char const* testName, int inNumTests);
        void onTestFixtureEnd(char const* testName, float secondsElapsed);
        void onTestStart(char const* testName);
        void onTestFailure(char const* file, int line, char const* testName, char const* failure);
        void onTestFailure(char const* file, int line, char const* testName, char const* failure, const char* message);
        void onTestEnd(char const* testName, float secondsElapsed);

        inline int getTestCount() const { return mTestCount; }
        inline int getFailureCount() const { return mFailureCount; }

        TestReporter* mTestReporter;
        int           mTestSuiteCount;
        int           mTestFixtureCount;
        int           mTestCount;
        int           mFailureCount;
        int           mExceptionTestCount;
        int           mExceptionFailureCount;
    };
} // namespace UnitTest

#endif // __CUNITTEST_TESTRESULTS_H__
