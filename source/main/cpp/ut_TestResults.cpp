#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_TestReporter.h"

namespace UnitTest
{
    TestResults::TestResults()
        : mTestReporter(nullptr)
        , mTestSuiteCount(0)
        , mTestFixtureCount(0)
        , mTestCount(0)
        , mFailureCount(0)
        , mExceptionTestCount(0)
        , mExceptionFailureCount(0)
    {
    }

    TestResults::TestResults(TestReporter* testReporter)
        : mTestReporter(testReporter)
        , mTestSuiteCount(0)
        , mTestFixtureCount(0)
        , mTestCount(0)
        , mFailureCount(0)
        , mExceptionTestCount(0)
        , mExceptionFailureCount(0)
    {
    }

    void TestResults::onTestSuiteStart(char const* testName, int inNumTests)
    {
        ++mTestSuiteCount;
        if (mTestReporter)
            mTestReporter->reportTestSuiteStart(inNumTests, testName);
    }

    void TestResults::onTestSuiteEnd(char const* testName, float secondsElapsed)
    {
        if (mTestReporter)
            mTestReporter->reportTestSuiteEnd(testName, secondsElapsed);
    }

    void TestResults::onTestFixtureStart(char const* testName, int inNumTests)
    {
        ++mTestFixtureCount;
        if (mTestReporter)
            mTestReporter->reportTestFixtureStart(inNumTests, testName);
    }

    void TestResults::onTestFixtureEnd(char const* testName, float secondsElapsed)
    {
        if (mTestReporter)
            mTestReporter->reportTestFixtureEnd(testName, secondsElapsed);
    }

    void TestResults::onTestStart(char const* testName)
    {
        ++mTestCount;
        if (mTestReporter)
            mTestReporter->reportTestStart(testName);
    }

    void TestResults::onTestFailure(char const* file, int line, char const* testName, char const* failure)
    {
        ++mFailureCount;
        if (mTestReporter)
            mTestReporter->reportFailure(file, line, testName, failure);
    }

    void TestResults::onTestFailure(char const* file, int line, char const* testName, char const* failure, const char* message)
    {
        ++mFailureCount;
        if (mTestReporter)
            mTestReporter->reportFailure(file, line, testName, failure, message);
    }

    void TestResults::onTestEnd(char const* testName, float secondsElapsed)
    {
        if (mTestReporter)
            mTestReporter->reportTestEnd(testName, secondsElapsed);
    }
} // namespace UnitTest

