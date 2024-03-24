#ifndef __CUNITTEST_TESTMACROS_H__
#define __CUNITTEST_TESTMACROS_H__

#ifdef UNITTEST
#    error UnitTest redefines UNITTEST
#endif

#define UNITTEST_SUITE_LIST           \
    namespace UnitTest                \
    {                                 \
        TestSuite* cUnitTest     = 0; \
        TestSuite* cUnitTestTail = 0; \
    }

#define UNITTEST_SUITE_RUN(Context, Reporter, NameOfList) UnitTest::TestAllRun(Context, Reporter, UnitTest::NameOfList);

#define UNITTEST_SUITE_BEGIN(NameOfSuite)                                                                               \
    namespace UnitTest                                                                                                  \
    {                                                                                                                   \
        extern TestSuite* cUnitTest;                                                                                    \
        extern TestSuite* cUnitTestTail;                                                                                \
    }                                                                                                                   \
    namespace Suite##NameOfSuite                                                                                        \
    {                                                                                                                   \
        UnitTest::TestSuite           gSuiteObject("Suite_" #NameOfSuite, __FILE__);                                    \
        UnitTest::AddSuiteToSuiteList gTestSuiteAddToList(UnitTest::cUnitTest, UnitTest::cUnitTestTail, &gSuiteObject); \
    }                                                                                                                   \
    namespace Suite##NameOfSuite

#define UNITTEST_SUITE_END

#define UNITTEST_FIXTURE(Name)                                                                                  \
    namespace nsTestFixture##Name                                                                               \
    {                                                                                                           \
        UnitTest::TestAllocator* FixtureAllocator = 0;                                                          \
        UnitTest::TestFixture    gFixtureInstance(#Name, __FILE__, __LINE__, &FixtureAllocator, &gSuiteObject); \
    }                                                                                                           \
    namespace nsTestFixture##Name

#define UNITTEST_FIXTURE_SETUP()                                                     \
    void                         FixtureSetup(UnitTest::TestResults& testResults);   \
    UnitTest::SetSetupForFixture gSetFixtureSetup(&gFixtureInstance, &FixtureSetup); \
    void                         FixtureSetup(UnitTest::TestResults& testResults)

#define UNITTEST_FIXTURE_TEARDOWN()                                                           \
    void                            FixtureTeardown(UnitTest::TestResults& testResults);      \
    UnitTest::SetTeardownForFixture gSetFixtureTeardown(&gFixtureInstance, &FixtureTeardown); \
    void                            FixtureTeardown(UnitTest::TestResults& testResults)

#define UNITTEST_TEST(Name)                                                                                       \
    void           TestRun_##Name(const char* testName, UnitTest::TestResults& testResults, int maxTestTimeInMs); \
    UnitTest::Test gTestInstance##Name(#Name, __FILE__, __LINE__, &TestRun_##Name, &gFixtureInstance);            \
    void           TestRun_##Name(const char* testName, UnitTest::TestResults& testResults, int maxTestTimeInMs)

#endif ///< __CUNITTEST_TESTMACROS_H__
