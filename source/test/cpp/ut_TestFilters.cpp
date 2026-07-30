#include "cunittest/cunittest.h"
#include "cunittest/ut_RecordingReporter.h"

using namespace UnitTest;

namespace
{
    void s_NoOpTestRun(const char* testName, TestResults& testResults, const float maxTestTimeInMs)
    {
        (void)testName;
        (void)testResults;
        (void)maxTestTimeInMs;
    }

    struct FilterTree
    {
        NullAllocator  mNullAllocator;
        TestAllocator* mFixtureAllocator;
        TestSuite      mSuite1;
        TestSuite      mSuite2;
        TestFixture    mSuite1Fixture1;
        TestFixture    mSuite1Fixture2;
        TestFixture    mSuite2Fixture1;
        Test           mSuite1Fixture1Test1;
        Test           mSuite1Fixture1Test2;
        Test           mSuite1Fixture2Test1;
        Test           mSuite2Fixture1Test1;
        Test           mSuite2Fixture1Test2;

        FilterTree()
            : mFixtureAllocator(&mNullAllocator)
            , mSuite1("Suite1", "suite1.cpp")
            , mSuite2("Suite2", "suite2.cpp")
            , mSuite1Fixture1("Fixture1", "suite1.cpp", 1, &mFixtureAllocator, &mSuite1)
            , mSuite1Fixture2("Fixture2", "suite1.cpp", 2, &mFixtureAllocator, &mSuite1)
            , mSuite2Fixture1("Fixture1", "suite2.cpp", 3, &mFixtureAllocator, &mSuite2)
            , mSuite1Fixture1Test1("Test1", "suite1.cpp", 10, s_NoOpTestRun, &mSuite1Fixture1)
            , mSuite1Fixture1Test2("Test2", "suite1.cpp", 11, s_NoOpTestRun, &mSuite1Fixture1)
            , mSuite1Fixture2Test1("Test1", "suite1.cpp", 12, s_NoOpTestRun, &mSuite1Fixture2)
            , mSuite2Fixture1Test1("Test1", "suite2.cpp", 13, s_NoOpTestRun, &mSuite2Fixture1)
            , mSuite2Fixture1Test2("Test2", "suite2.cpp", 14, s_NoOpTestRun, &mSuite2Fixture1)
        {
            mSuite1.mSuiteNext = &mSuite2;
        }
    };

    void s_SetRunFlag(TestSuite* suiteList, bool run)
    {
        TestSuite* suite = suiteList;
        while (suite != 0)
        {
            suite->mRun = run;

            TestFixture* fixture = suite->mFixtureListHead;
            while (fixture != 0)
            {
                fixture->mRun = run;

                Test* test = fixture->mTestListHead;
                while (test != 0)
                {
                    test->mRun = run;
                    test       = test->mTestNext;
                }

                fixture = fixture->mFixtureNext;
            }

            suite = suite->mSuiteNext;
        }
    }
}

UNITTEST_SUITE_BEGIN(TestFilters)
{
    UNITTEST_FIXTURE(fixture)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(EmptyFilterLeavesAllNodesRunnable)
        {
            FilterTree tree;

            s_SetRunFlag(&tree.mSuite1, true);
            g_IterateFilter(nullptr, &tree.mSuite1);

            CHECK_TRUE(tree.mSuite1.mRun);
            CHECK_TRUE(tree.mSuite2.mRun);
            CHECK_EQUAL(2, tree.mSuite1.mActiveFixtureCount);
            CHECK_EQUAL(1, tree.mSuite2.mActiveFixtureCount);
            CHECK_TRUE(tree.mSuite1Fixture1.mRun);
            CHECK_TRUE(tree.mSuite1Fixture2.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1.mRun);
            CHECK_EQUAL(2, tree.mSuite1Fixture1.mActiveTestCount);
            CHECK_EQUAL(1, tree.mSuite1Fixture2.mActiveTestCount);
            CHECK_EQUAL(2, tree.mSuite2Fixture1.mActiveTestCount);
            CHECK_TRUE(tree.mSuite1Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite1Fixture1Test2.mRun);
            CHECK_TRUE(tree.mSuite1Fixture2Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test2.mRun);
        }

        UNITTEST_TEST(IteratorEnablesOnlyMatchingFixtureAndSuiteUnion)
        {
            FilterTree tree;

            s_SetRunFlag(&tree.mSuite1, false);
            g_IterateFilter("Suite1/Fixture1,Suite2", &tree.mSuite1);

            CHECK_TRUE(tree.mSuite1.mRun);
            CHECK_TRUE(tree.mSuite2.mRun);
            CHECK_EQUAL(1, tree.mSuite1.mActiveFixtureCount);
            CHECK_EQUAL(1, tree.mSuite2.mActiveFixtureCount);
            CHECK_TRUE(tree.mSuite1Fixture1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture2.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1.mRun);
            CHECK_EQUAL(2, tree.mSuite1Fixture1.mActiveTestCount);
            CHECK_EQUAL(0, tree.mSuite1Fixture2.mActiveTestCount);
            CHECK_EQUAL(2, tree.mSuite2Fixture1.mActiveTestCount);
            CHECK_TRUE(tree.mSuite1Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite1Fixture1Test2.mRun);
            CHECK_FALSE(tree.mSuite1Fixture2Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test2.mRun);
        }

        UNITTEST_TEST(IteratorEnablesOnlyMatchingTestAndSuiteUnion)
        {
            FilterTree tree;

            s_SetRunFlag(&tree.mSuite1, false);
            g_IterateFilter("Suite1/Fixture1/Test1,Suite2", &tree.mSuite1);

            CHECK_TRUE(tree.mSuite1.mRun);
            CHECK_TRUE(tree.mSuite2.mRun);
            CHECK_EQUAL(1, tree.mSuite1.mActiveFixtureCount);
            CHECK_EQUAL(1, tree.mSuite2.mActiveFixtureCount);
            CHECK_TRUE(tree.mSuite1Fixture1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture2.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1.mRun);
            CHECK_EQUAL(1, tree.mSuite1Fixture1.mActiveTestCount);
            CHECK_EQUAL(0, tree.mSuite1Fixture2.mActiveTestCount);
            CHECK_EQUAL(2, tree.mSuite2Fixture1.mActiveTestCount);
            CHECK_TRUE(tree.mSuite1Fixture1Test1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture1Test2.mRun);
            CHECK_FALSE(tree.mSuite1Fixture2Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test2.mRun);
        }

        UNITTEST_TEST(OverlappingEntriesEnableTheUnionOfMatches)
        {
            FilterTree tree;

            s_SetRunFlag(&tree.mSuite1, false);
            g_IterateFilter("Suite1/Fixture1/Test1,Suite1", &tree.mSuite1);

            CHECK_TRUE(tree.mSuite1.mRun);
            CHECK_FALSE(tree.mSuite2.mRun);
            CHECK_EQUAL(2, tree.mSuite1.mActiveFixtureCount);
            CHECK_EQUAL(0, tree.mSuite2.mActiveFixtureCount);
            CHECK_TRUE(tree.mSuite1Fixture1.mRun);
            CHECK_TRUE(tree.mSuite1Fixture2.mRun);
            CHECK_FALSE(tree.mSuite2Fixture1.mRun);
            CHECK_EQUAL(2, tree.mSuite1Fixture1.mActiveTestCount);
            CHECK_EQUAL(1, tree.mSuite1Fixture2.mActiveTestCount);
            CHECK_EQUAL(0, tree.mSuite2Fixture1.mActiveTestCount);
            CHECK_TRUE(tree.mSuite1Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite1Fixture1Test2.mRun);
            CHECK_TRUE(tree.mSuite1Fixture2Test1.mRun);
            CHECK_FALSE(tree.mSuite2Fixture1Test1.mRun);
            CHECK_FALSE(tree.mSuite2Fixture1Test2.mRun);
        }

        UNITTEST_TEST(TestAllRunRecomputesRunFlagsPerInvocation)
        {
            FilterTree tree;

            TestContext context;
            context.mTestFilter = "Suite1/Fixture1/Test1";

            RecordingReporter reporter1;
            CHECK_EQUAL(0, TestAllRun(context, reporter1, &tree.mSuite1, 0.0f));
            CHECK_EQUAL(1, reporter1.testRunCount);
            CHECK_TRUE(tree.mSuite1.mRun);
            CHECK_FALSE(tree.mSuite2.mRun);
            CHECK_EQUAL(1, tree.mSuite1.mActiveFixtureCount);
            CHECK_EQUAL(0, tree.mSuite2.mActiveFixtureCount);
            CHECK_TRUE(tree.mSuite1Fixture1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture2.mRun);
            CHECK_EQUAL(1, tree.mSuite1Fixture1.mActiveTestCount);
            CHECK_EQUAL(0, tree.mSuite1Fixture2.mActiveTestCount);
            CHECK_EQUAL(0, tree.mSuite2Fixture1.mActiveTestCount);
            CHECK_TRUE(tree.mSuite1Fixture1Test1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture1Test2.mRun);

            context.mTestFilter = "Suite2";

            RecordingReporter reporter2;
            CHECK_EQUAL(0, TestAllRun(context, reporter2, &tree.mSuite1, 0.0f));
            CHECK_EQUAL(2, reporter2.testRunCount);
            CHECK_FALSE(tree.mSuite1.mRun);
            CHECK_TRUE(tree.mSuite2.mRun);
            CHECK_EQUAL(0, tree.mSuite1.mActiveFixtureCount);
            CHECK_EQUAL(1, tree.mSuite2.mActiveFixtureCount);
            CHECK_FALSE(tree.mSuite1Fixture1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture2.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1.mRun);
            CHECK_EQUAL(0, tree.mSuite1Fixture1.mActiveTestCount);
            CHECK_EQUAL(0, tree.mSuite1Fixture2.mActiveTestCount);
            CHECK_EQUAL(2, tree.mSuite2Fixture1.mActiveTestCount);
            CHECK_FALSE(tree.mSuite1Fixture1Test1.mRun);
            CHECK_FALSE(tree.mSuite1Fixture1Test2.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test1.mRun);
            CHECK_TRUE(tree.mSuite2Fixture1Test2.mRun);
        }
    }
}
UNITTEST_SUITE_END