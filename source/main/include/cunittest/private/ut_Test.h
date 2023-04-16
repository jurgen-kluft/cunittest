#ifndef __CUNITTEST_TEST_H__
#define __CUNITTEST_TEST_H__

namespace UnitTest
{
    class TestResults;
    class TestContext;
    class TestAllocator;
    
    struct Test;
    struct TestFixture;
    struct TestSuite;

    typedef void (*TestSetup)(TestResults& testResults_);
    typedef void (*TestTeardown)(TestResults& testResults_);
    typedef void (*TestRun)(const char* testName, TestResults& testResults_, int const maxTestTimeInMs);

    struct Test
    {
        Test(char const* testName, char const* filename, const int lineNumber, TestRun run, TestFixture* fixture);

        char const* const mName;
        char const* const mFilename;
        int const         mLineNumber;
        mutable bool      mTimeConstraintExempt;

        TestRun mTestRun;
        Test*   mTestNext;
    };

    struct TestFixture
    {
        TestFixture(const char* inName, const char* inFilename, const int inLineNumber, TestAllocator** inAllocator, TestSuite* inSuite);

        Test* mTestListHead;
        Test* mTestListTail;

        char const* const mName;
        char const* const mFilename;
        int const         mLineNumber;
        mutable bool      mTimeConstraintExempt;

        TestSetup    mSetup;
        TestTeardown mTeardown;

        TestAllocator** mAllocator;
        TestFixture*    mFixtureNext;
    };

    struct TestSuite
    {
        TestSuite(const char* inName, const char* inFilename);

        char const* const mName;
        char const* const mFilename;
        TestFixture*      mFixtureListHead;
        TestFixture*      mFixtureListTail;
        TestSuite*        mSuiteNext;
    };

    void TestFixtureRun(TestSuite* suite, TestFixture* fixture, TestContext& context, TestResults& results, int maxTestTimeInMs);

    class AddTestToFixture
    {
    public:
        AddTestToFixture(TestFixture* fixture, Test* test)
        {
            if (fixture->mTestListHead == nullptr)
                fixture->mTestListHead = test;
            else
                fixture->mTestListTail->mTestNext = test;

            fixture->mTestListTail = test;
        }
    };

    class SetRunForTest
    {
    public:
        SetRunForTest(Test* test, TestRun run) { test->mTestRun = run; }
    };

    class SetSetupForFixture
    {
    public:
        SetSetupForFixture(TestFixture* fixture, TestSetup setup) { fixture->mSetup = setup; }
    };
    class SetTeardownForFixture
    {
    public:
        SetTeardownForFixture(TestFixture* fixture, TestTeardown teardown) { fixture->mTeardown = teardown; }
    };

    class AddSuiteToSuiteList
    {
    public:
        AddSuiteToSuiteList(TestSuite*& head, TestSuite*& tail, TestSuite* suite)
        {
            if (head == nullptr)
                head = suite;
            else
                tail->mSuiteNext = suite;

            tail = suite;
        }
    };

} // namespace UnitTest

#endif ///< __CUNITTEST_TEST_H__
