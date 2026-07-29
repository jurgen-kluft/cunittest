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
        {
            inSuite->mFixtureListHead = this;
            inSuite->mFixtureListTail = this;
        }
        else
        {
            inSuite->mFixtureListTail->mFixtureNext = this;
            inSuite->mFixtureListTail               = this;
        }
    }

    TestSuite::TestSuite(const char* inName, const char* inFilename)
        : mName(inName)
        , mFilename(inFilename)
        , mFixtureListHead(0)
        , mFixtureListTail(0)
        , mSuiteNext(0)
    {
    }

    int TestAllRun(TestContext& context, TestReporter& reporter, TestSuite* inSuiteList, const float maxTestTimeInMs)
    {
        TestResults result(&reporter);

        time_t overallTime = g_TimeStart();

        TestSuite* suiteIter = inSuiteList;
        while (suiteIter != 0)
        {
            if (g_ShouldRunSuite(context.mTestFilter, suiteIter->mName))
            {
                int numTests = 0;

                TestFixture* curTestFixture = suiteIter->mFixtureListHead;
                while (curTestFixture != 0)
                {
                    if (g_ShouldRunFixture(context.mTestFilter, curTestFixture->mName))
                    {
                        ++numTests;
                    }
                    curTestFixture = curTestFixture->mFixtureNext;
                }

                context.mObserver->BeginSuite(suiteIter->mFilename, suiteIter->mName);
                {
                    time_t suiteStartTime = g_TimeStart();
                    result.onTestSuiteStart(suiteIter->mName, numTests);

                    curTestFixture = suiteIter->mFixtureListHead;
                    while (curTestFixture != 0)
                    {
                        if (g_ShouldRunFixture(context.mTestFilter, curTestFixture->mName))
                        {
                            TestFixtureRun(suiteIter, curTestFixture, context, result, maxTestTimeInMs);
                        }
                        curTestFixture = curTestFixture->mFixtureNext;
                    }

                    result.onTestSuiteEnd(suiteIter->mName, (float)g_GetElapsedTimeInMs(suiteStartTime) / 1000.0f);
                }
                context.mObserver->EndSuite();
            }
            suiteIter = suiteIter->mSuiteNext;
        }

        float const secondsElapsed = (float)g_GetElapsedTimeInMs(overallTime) / 1000.0f;
        reporter.reportSummary(secondsElapsed, result.mFailureCount, result.mTestCount);

        return result.mFailureCount;
    }

} // namespace UnitTest
