#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_ReportAssert.h"
#include "cunittest/private/ut_Config.h"

UNITTEST_SUITE_BEGIN(TestCpp)
{
	UNITTEST_FIXTURE(main)
	{
		static int instanceCount = 0;

		UNITTEST_FIXTURE_SETUP()
		{
			instanceCount++;
			CHECK(instanceCount > 0);
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
			CHECK_EQUAL(1, instanceCount);
			instanceCount--;
		}

		UNITTEST_TEST(ValidCheckSucceeds)
		{
			bool const b = true;
			CHECK(b);
		}

		UNITTEST_TEST(CheckWorksWithPointers)
		{
			void* p = (void *)0x100;
			CHECK(p);
			CHECK(p != 0);
		}

		UNITTEST_TEST(ValidCheckEqualSucceeds)
		{
			int const x = 3;
			int const y = 3;
			CHECK_EQUAL(x, y);
		}

		UNITTEST_TEST(CheckEqualWorksWithPointers)
		{
			void* p = (void *)0;
			CHECK_EQUAL ((void*)0, p);
		}

		UNITTEST_TEST(ValidCheckCloseSucceeds)
		{
			CHECK_CLOSE(2.0f, 2.001f, 0.01f);
			CHECK_CLOSE(2.001f, 2.0f, 0.01f);
		}

		UNITTEST_TEST(ArrayCloseSucceeds)
		{
			float const a1[] = {1, 2, 3};
			float const a2[] = {1, 2.01f, 3};
			CHECK_ARRAY_CLOSE (a1, a2, 3, 0.1f);
		}

		// UNITTEST_TEST(CheckAssertSucceeds)
		// {
		// 	CHECK_ASSERT(UnitTest::ReportAssert("desc", "file", 0));
		// }

		UNITTEST_TEST(CheckThrowMacroSucceedsOnCorrectException)
		{
			struct TestException {};
			CHECK_THROW(UT_THROW1(TestException()), TestException);
		}

		// UNITTEST_TEST(CheckThrowMacroFailsOnMissingException)
		// {
		// 	class NoThrowTest : public UnitTest::Test
		// 	{
		// 	public:
		// 		NoThrowTest() : Test("nothrow") {}

		// 		void DontThrow() const
		// 		{
		// 		}

		// 		virtual void runImpl(UnitTest::TestResults& testResults_) const
		// 		{
		// 			CHECK_THROW(DontThrow(), int);
		// 		}
		// 	};

		// 	UnitTest::TestResults results;

		// 	NoThrowTest test;
		// 	test.run(results, 1);
		// 	CHECK_EQUAL(1, results.getFailureCount());
		// }

		// UNITTEST_TEST(CheckThrowMacroFailsOnWrongException)
		// {
		// 	class WrongThrowTest : public UnitTest::Test
		// 	{
		// 	public:
		// 		WrongThrowTest() : Test("wrongthrow") {}

		// 		virtual void runImpl(UnitTest::TestResults& testResults_) const
		// 		{
		// 			CHECK_THROW(UT_THROW1("oops"), int);
		// 		}
		// 	};

		// 	UnitTest::TestResults results;

		// 	WrongThrowTest test;
		// 	test.run(results, 1);
		// 	CHECK_EQUAL(1, results.getFailureCount());
		// }

	}
}
UNITTEST_SUITE_END
