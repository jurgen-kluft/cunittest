#include "cunittest/private/ut_Config.h"
#include "cunittest/private/ut_Test.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_TimeHelpers.h"
#include "cunittest/private/ut_AssertException.h"
#include "cunittest/private/ut_StringBuilder.h"
#include "cunittest/private/ut_Stdout.h"
#include "cunittest/private/ut_Utils.h"

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
        , mSuiteNext(0)
    {

    }

} // namespace UnitTest
