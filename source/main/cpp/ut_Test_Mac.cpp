#ifdef TARGET_MAC

#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_Test.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_TimeHelpers.h"
#include "cunittest/private/ut_Exception.h"
#include "cunittest/private/ut_StringBuilder.h"
#include "cunittest/private/ut_Stdout.h"
#include "cunittest/private/ut_Utils.h"

#include <exception>

namespace UnitTest
{
    class AssertException : public std::exception
    {
        enum ESettings
        {
            DESCRIPTION_MAX_STR_LENGTH = 512,
            FILENAME_MAX_STR_LENGTH    = 256
        };

    public:
        AssertException(char const* description, char const* filename, const int lineNumber);

        char mDescription[DESCRIPTION_MAX_STR_LENGTH];
        char mFilename[FILENAME_MAX_STR_LENGTH];
        int  mLineNumber;
    };

    AssertException::AssertException(char const* description, char const* filename, const int lineNumber)
        : mLineNumber(lineNumber)
    {
        gStringCopy(mDescription, description, FILENAME_MAX_STR_LENGTH);
        gStringCopy(mFilename, filename, DESCRIPTION_MAX_STR_LENGTH);
    }

    void ReportAssert(char const* description, char const* filename, int const lineNumber) { UT_THROW1(AssertException(description, filename, lineNumber)); }

    void TestTestRun(Test* test, TestContext& context, TestResults& results, int const maxTestTimeInMs)
    {
        unsigned int testTime = g_TimeStart();

        results.onTestStart(test->mName);

        try
        {
            test->mTestRun(test->mName, results, maxTestTimeInMs);
        }
        catch (AssertException const& e)
        {
            results.onTestFailure(e.filename(), e.lineNumber(), test->mName, e.what());
        }
        catch (std::exception const& e)
        {
            StringBuilder stringBuilder(context.mAllocator);
            stringBuilder << "Unhandled exception: " << e.what();
            results.onTestFailure(test->mFilename, test->mLineNumber, test->mName, stringBuilder.getText());
        }
        catch (...)
        {
            results.onTestFailure(test->mFilename, test->mLineNumber, test->mName, "Unhandled exception: Crash!");
        }
        const int testTimeInMs = (int)((float)g_GetElapsedTimeInMs(testTime) / 1000.0f);
        if (maxTestTimeInMs > 0 && testTimeInMs > maxTestTimeInMs && !test->mTimeConstraintExempt)
        {
            StringBuilder stringBuilder(context.mAllocator);
            stringBuilder << "Global time constraint failed. Expected under ";
            stringBuilder << maxTestTimeInMs;
            stringBuilder << "ms but took ";
            stringBuilder << testTimeInMs;
            stringBuilder << "ms.";

            results.onTestFailure(test->mFilename, test->mLineNumber, test->mName, stringBuilder.getText());
        }
        results.onTestEnd(test->mName, testTimeInMs / 1000.0f);
    }

    void TestFixtureRun(TestSuite* suite, TestFixture* fixture, TestContext& context, TestResults& results, int maxTestTimeInMs)
    {
        enum EStep
        {
            FIXTURE_SETUP,
            FIXTURE_UNITTESTS,
            FIXTURE_TEARDOWN,
        };

        // The fixture allocator will track the number of allocations and deallocations
        TestAllocatorEx fixtureAllocator(context.mAllocator);
        *fixture->mAllocator = &fixtureAllocator;

        int numTests = 0;
        if (fixture->mTestListHead != 0)
        {
            Test* curTest = fixture->mTestListHead;
            while (curTest != 0)
            {
                numTests++;
                curTest = curTest->mTestNext;
            }
        }

        unsigned int testTime = g_TimeStart();

        results.onTestFixtureStart(fixture->mName, numTests);

        EStep step = FIXTURE_SETUP;
        try
        {
            // Remember allocation count X
            int iAllocCntX       = fixtureAllocator.GetNumAllocations();
            int iMemLeakCnt      = 0;
            int iExtraDeallocCnt = 0;

            if (fixture->mSetup != 0)
            {
                fixture->mSetup(results);
            }

            if (fixture->mTestListHead != 0)
            {
                step          = FIXTURE_UNITTESTS;
                Test* curTest = fixture->mTestListHead;
                while (curTest != 0)
                {
                    // Remember allocation count Y
                    int iAllocCntY = fixtureAllocator.GetNumAllocations();

                    curTest->mTestRun(curTest->mName, results, maxTestTimeInMs);

                    // Compare allocation count with Y
                    // If different => memory leak error
                    if (iAllocCntY != fixtureAllocator.GetNumAllocations())
                    {
                        int iAllocCountDifference = (fixtureAllocator.GetNumAllocations() - iAllocCntY);

                        StringBuilder str(context.mAllocator);
                        if (iAllocCountDifference > 0)
                        {
                            iMemLeakCnt += iAllocCountDifference;
                            str << "memory leak detected, leaked memory allocations: ";
                            str << iAllocCountDifference;
                        }
                        else
                        {
                            iExtraDeallocCnt += -1 * iAllocCountDifference;
                            str << "extra memory deallocations detected, unmatching deallocations: ";
                            str << -1 * iAllocCountDifference;
                        }

                        results.onTestFailure(curTest->mFilename, curTest->mLineNumber, curTest->mName, str.getText());
                    }
                    curTest = curTest->mTestNext;
                }
            }

            step = FIXTURE_TEARDOWN;
            if (fixture->mTeardown != 0)
                fixture->mTeardown(results);

            // Compare allocation count with X
            // If different => Fixture memory leak error (probably the combination of Setup() and Teardown()
            if (iAllocCntX != (fixtureAllocator.GetNumAllocations() - iMemLeakCnt))
            {
                StringBuilder str(context.mAllocator);

                str << "memory leak detected in setup()/teardown(), leaked memory allocations: ";
                str << iMemLeakCnt;

                results.onTestFailure(fixture->mFilename, fixture->mLineNumber, fixture->mName, str.getText());
            }

            if (iAllocCntX != (fixtureAllocator.GetNumAllocations() - iExtraDeallocCnt))
            {
                StringBuilder str(context.mAllocator);

                str << "extra deallocations detected in setup()/teardown(), extra deallocations: ";
                str << iExtraDeallocCnt;

                results.onTestFailure(fixture->mFilename, fixture->mLineNumber, fixture->mName, str.getText());
            }
        }
        catch (std::exception const& e)
        {
            StringBuilder stringBuilder(context.mAllocator);
            if (step == FIXTURE_SETUP)
                stringBuilder << "Unhandled exception in setup of fixture " << fixture->mName;
            else if (step == FIXTURE_TEARDOWN)
                stringBuilder << "Unhandled exception in teardown of fixture " << fixture->mName;
            else
                stringBuilder << "Unhandled exception in fixture " << fixture->mName;

            stringBuilder << " : " << e.what();
            results.onTestFailure(fixture->mFilename, fixture->mLineNumber, fixture->mName, stringBuilder.getText());
        }
        catch (...)
        {
            StringBuilder stringBuilder(context.mAllocator);
            if (step == FIXTURE_SETUP)
                stringBuilder << "Unhandled exception in setup of fixture " << fixture->mName;
            else if (step == FIXTURE_TEARDOWN)
                stringBuilder << "Unhandled exception in teardown of fixture " << fixture->mName;
            else
                stringBuilder << "Unhandled exception in fixture " << fixture->mName;

            results.onTestFailure(fixture->mFilename, fixture->mLineNumber, fixture->mName, stringBuilder.getText());
        }

        const int testTimeInMs = (int)((float)g_GetElapsedTimeInMs(testTime) / 1000.0f);
        if (maxTestTimeInMs > 0 && testTimeInMs > maxTestTimeInMs && !fixture->mTimeConstraintExempt)
        {
            StringBuilder stringBuilder(context.mAllocator);
            stringBuilder << "Fixture time constraint failed. Expected under ";
            stringBuilder << maxTestTimeInMs;
            stringBuilder << "ms but took ";
            stringBuilder << testTimeInMs;
            stringBuilder << "ms.";

            results.onTestFailure(fixture->mFilename, fixture->mLineNumber, fixture->mName, stringBuilder.getText());
        }
        results.onTestFixtureEnd(fixture->mName, testTimeInMs / 1000.0f);
    }
} // namespace UnitTest

#endif
