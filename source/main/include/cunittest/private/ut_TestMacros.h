#ifndef __CUNITTEST_TESTMACROS_H__
#define __CUNITTEST_TESTMACROS_H__

#ifdef UNITTEST
#error UnitTest redefines UNITTEST
#endif

#define UNITTEST_SUITE_LIST(NameOfList)  \
    namespace UnitTest                   \
    {                                    \
        TestSuite* NameOfList       = 0; \
        TestSuite* NameOfList##Tail = 0; \
    }

#define UNITTEST_SUITE_DECLARE(NameOfList, NameOfSuite)                                                                     \
    namespace Suite##NameOfSuite                                                                                            \
    {                                                                                                                       \
        extern UnitTest::TestSuite    gSuiteObject;                                                                         \
        UnitTest::AddSuiteToSuiteList gTestSuiteAddToList(UnitTest::NameOfList, UnitTest::NameOfList##Tail, &gSuiteObject); \
    }

#define UNITTEST_SUITE_RUN(Context, Reporter, NameOfList) UnitTest::runAllTests(Context, Reporter, UnitTest::NameOfList);

#define UNITTEST_SUITE_BEGIN(NameOfSuite)                                  \
    namespace Suite##NameOfSuite                                           \
    {                                                                      \
        UnitTest::TestSuite gSuiteObject("Suite_" #NameOfSuite, __FILE__); \
    }                                                                      \
    namespace Suite##NameOfSuite

#define UNITTEST_SUITE_END

#define UNITTEST_FIXTURE(Name)                                                                                      \
    namespace nsTestFixture##Name                                                                                   \
    {                                                                                                               \
        static UnitTest::TestAllocator* TestAllocator = 0;                                                          \
        UnitTest::TestFixture           gFixtureInstance(#Name, __FILE__, __LINE__, &TestAllocator, &gSuiteObject); \
    }                                                                                                               \
    namespace nsTestFixture##Name

#define UNITTEST_FIXTURE_SETUP()                                                     \
    void                         FixtureSetup(UnitTest::TestResults& testResults);   \
    UnitTest::SetSetupForFixture gSetFixtureSetup(&gFixtureInstance, &FixtureSetup); \
    void                         FixtureSetup(UnitTest::TestResults& testResults)

#define UNITTEST_FIXTURE_TEARDOWN()                                                           \
    void                            FixtureTeardown(UnitTest::TestResults& testResults);      \
    UnitTest::SetTeardownForFixture gSetFixtureTeardown(&gFixtureInstance, &FixtureTeardown); \
    void                            FixtureTeardown(UnitTest::TestResults& testResults)

#define UNITTEST_TEST(Name)                                                                                    \
    namespace nsTest##Name                                                                                     \
    {                                                                                                          \
        void           TestRun(const char* testName, UnitTest::TestResults& testResults, int maxTestTimeInMs); \
        UnitTest::Test gTestInstance(#Name, __FILE__, __LINE__, &TestRun, &gFixtureInstance);                  \
    }                                                                                                          \
    void nsTest##Name::TestRun(const char* testName, UnitTest::TestResults& testResults, int maxTestTimeInMs)

#endif ///< __CUNITTEST_TESTMACROS_H__
