#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_TestRunner.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_Test.h"
#include "cunittest/private/ut_TestState.h"
#include "cunittest/private/ut_TestMacros.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_TestReporterStdout.h"
#include "cunittest/private/ut_TimeHelpers.h"
#include "cunittest/private/ut_StringBuilder.h"
#include "cunittest/private/ut_Stdout.h"

namespace UnitTest
{
    int runAllTests(TestContext& context, TestReporter& reporter, TestSuite* inSuiteList, int maxTestTimeInMs)
    {
        TestState testState;
        TestResults result(&reporter, &testState);

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
