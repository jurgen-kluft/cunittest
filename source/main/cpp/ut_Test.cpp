#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_Test.h"
#include "cunittest/private/ut_TestMacros.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_TestReporterStdout.h"
#include "cunittest/private/ut_TimeHelpers.h"
#include "cunittest/private/ut_StringBuilder.h"
#include "cunittest/private/ut_Stdout.h"

namespace UnitTest
{
    static NullObserver  sNullObserver;
    static NullAllocator sNullAllocator;

    TestContext::TestContext()
        : mAllocator(&sNullAllocator)
        , mObserver(&sNullObserver)
    {
    }

    Test::Test(char const* testName, char const* filename, const int lineNumber, TestRun run, TestFixture* fixture)
        : mName(testName)
        , mFilename(filename)
        , mLineNumber(lineNumber)
        , mTimeConstraintExempt(false)
        , mTestRun(run)
        , mTestNext(0)
    {
        if (fixture != 0)
        {
            // add this test to the fixture's list
            if (fixture->mTestListHead == 0)
            {
                fixture->mTestListHead = this;
            }
            else
            {
                fixture->mTestListTail->mTestNext = this;
            }
            fixture->mTestListTail = this;
        }
    }

    TestFixture::TestFixture(const char* inName, const char* inFilename, const int inLineNumber, TestAllocator** inAllocator, TestSuite* inSuite)
        : mTestListHead(0)
        , mTestListTail(0)
        , mName(inName)
        , mFilename(inFilename)
        , mLineNumber(inLineNumber)
        , mTimeConstraintExempt(false)
        , mSetup(0)
        , mTeardown(0)
        , mAllocator(inAllocator)
        , mFixtureNext(0)
    {
        if (inSuite->mFixtureListHead == nullptr)
            inSuite->mFixtureListHead = this;
        else
            inSuite->mFixtureListTail->mFixtureNext = this;

        inSuite->mFixtureListTail = this;
    }

    TestSuite::TestSuite(const char* inName, const char* inFilename)
        : mName(inName)
        , mFilename(inFilename)
        , mFixtureListHead(0)
        , mFixtureListTail(0)
    {

    }

    int TestAllRun(TestContext& context, TestReporter& reporter, TestSuite* inSuiteList, const float maxTestTimeInMs)
    {
        TestResults result(&reporter);

        time_t overallTime = g_TimeStart();

        TestSuite* suiteList = inSuiteList;
        while (suiteList != 0)
        {
            int        numTests     = 0;
            TestSuite* curTestSuite = suiteList;

            TestFixture* curTestFixture = curTestSuite->mFixtureListHead;
            while (curTestFixture != 0)
            {
                ++numTests;
                curTestFixture = curTestFixture->mFixtureNext;
            }

            context.mObserver->BeginSuite(curTestSuite->mFilename, curTestSuite->mName);
            {
                time_t suiteStartTime = g_TimeStart();
                result.onTestSuiteStart(curTestSuite->mName, numTests);

                curTestFixture = curTestSuite->mFixtureListHead;
                while (curTestFixture != 0)
                {
                    TestFixtureRun(curTestSuite, curTestFixture, context, result, maxTestTimeInMs);
                    curTestFixture = curTestFixture->mFixtureNext;
                }

                result.onTestSuiteEnd(curTestSuite->mName, (float)g_GetElapsedTimeInMs(suiteStartTime) / 1000.0f);
            }
            context.mObserver->EndSuite();

            suiteList = suiteList->mSuiteNext;
        }

        float const secondsElapsed = (float)g_GetElapsedTimeInMs(overallTime) / 1000.0f;
        reporter.reportSummary(secondsElapsed, result.mFailureCount, result.mTestCount);

        return result.mFailureCount;
    }

} // namespace UnitTest
