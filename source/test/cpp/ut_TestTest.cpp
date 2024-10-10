#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_TimeHelpers.h"

using namespace UnitTest;

/*
UNITTEST_SUITE_BEGIN(TestTest)
{
	UNITTEST_FIXTURE(fixture)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(PassingTestHasNoFailures)
		{
			class PassingTest : public Test
			{
			public:
				PassingTest() : Test("passing") {}
				virtual void runImpl(TestResults& testResults_) const
				{
					CHECK(true);
				}
			};

			TestResults results;
			PassingTest().run(results, 1);
			CHECK_EQUAL(0, results.getFailureCount());
		}


		UNITTEST_TEST(FailingTestHasFailures)
		{
			class FailingTest : public Test
			{
			public:
				FailingTest() : Test("failing") {}
				virtual void runImpl(TestResults& testResults_) const
				{
					CHECK(false);
				}
			};

			TestResults results;
			FailingTest().run(results, 1);
			CHECK_EQUAL(1, results.getFailureCount());
		}

		UNITTEST_TEST(ThrowingTestsAreReportedAsFailures)
		{
			class CrashingTest : public Test
			{
			public:
				CrashingTest() : Test("throwing") {}
				virtual void runImpl(TestResults&) const
				{
					UT_THROW1("Blah");
				}
			};

			TestResults results;
			CrashingTest().run(results, 1);
			CHECK_EQUAL(1, results.getFailureCount());
		}

		UNITTEST_TEST(CrashingTestsAreReportedAsFailures)
		{
			class CrashingTest : public Test
			{
			public:
				CrashingTest() : Test("crashing") {}
				virtual void runImpl(TestResults&) const
				{
					reinterpret_cast< void (*)() >(0)();
				}
			};

			//TestResults results;
			//CrashingTest().run(results, 0);
			//CHECK_EQUAL(1, results.getFailureCount());
		}
	}
}
UNITTEST_SUITE_END

*/