#include "cunittest/xunittest.h"
#include "cunittest/ut_RecordingReporter.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestCheckMacros)
{
	UNITTEST_FIXTURE(fixture)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(CheckSucceedsOnTrue)
		{
			bool failure = true;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK (true);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (!failure);
		}

		UNITTEST_TEST(CheckFailsOnFalse)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK (false);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(FailureReportsCorrectTestName)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				CHECK (false);
			}

			CHECK_EQUAL (mTestName, reporter.lastFailedTest);
		}

		UNITTEST_TEST(CheckFailureIncludesCheckContents)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				const bool yaddayadda = false;
				CHECK (yaddayadda);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "yaddayadda"));
		}

		int ThrowingFunction()
		{
			UT_THROW1("Doh");
		}

		UNITTEST_TEST(CheckFailsOnException)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK (ThrowingFunction() == 1);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckFailureBecauseOfExceptionIncludesCheckContents)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				CHECK (ThrowingFunction() == 1);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "ThrowingFunction() == 1"));
		}

		UNITTEST_TEST(CheckEqualSuceedsOnEqual)
		{
			bool failure = true;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_EQUAL (1, 1);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (!failure);
		}

		UNITTEST_TEST(CheckEqualSuceedsWithStrings)
		{
			char string1[] = "xunittest";
			char string2[] = "xunittest";

			bool failure = true;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_EQUAL (string1, string2);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (!failure);
		}

		UNITTEST_TEST(CheckEqualFailsOnNotEqual)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_EQUAL (1, 2);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckEqualFailsOnException)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_EQUAL (ThrowingFunction(), 1);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckEqualFailureContainsCorrectInfo)
		{
			int line = 0;
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				CHECK_EQUAL (1, 123);    line = __LINE__;
			}

			CHECK_EQUAL ("CheckEqualFailureContainsCorrectInfo", reporter.lastFailedTest);
			CHECK_EQUAL (__FILE__, reporter.lastFailedFile);
			CHECK_EQUAL (line, reporter.lastFailedLine);
		}

		UNITTEST_TEST(CheckEqualFailureBecauseOfExceptionIncludesCheckContents)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				CHECK_EQUAL (ThrowingFunction(), 123);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "ThrowingFunction()"));
			CHECK (gStringFind(reporter.lastFailedMessage, "123"));
		}

		int g_sideEffect = 0;
		int FunctionWithSideEffects()
		{
			++g_sideEffect;
			return 1;
		}

		UNITTEST_TEST(CheckEqualDoesNotHaveSideEffectsWhenPassing)
		{
			g_sideEffect = 0;
			{
				UnitTest::TestResults testResults_;
				CHECK_EQUAL (1, FunctionWithSideEffects());
			}
			CHECK_EQUAL (1, g_sideEffect);
		}

		UNITTEST_TEST(CheckEqualDoesNotHaveSideEffectsWhenFailing)
		{
			g_sideEffect = 0;
			{
				UnitTest::TestResults testResults_;
				CHECK_EQUAL (2, FunctionWithSideEffects());
			}
			CHECK_EQUAL (1, g_sideEffect);
		}


		UNITTEST_TEST(CheckCloseSuceedsOnEqual)
		{
			bool failure = true;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_CLOSE (1.0f, 1.001f, 0.01f);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (!failure);
		}

		UNITTEST_TEST(CheckCloseFailsOnNotEqual)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_CLOSE (1.0f, 1.1f, 0.01f);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckCloseFailsOnException)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				CHECK_CLOSE ((float)ThrowingFunction(), 1.0001f, 0.1f);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckCloseFailureContainsCorrectInfo)
		{
			int line = 0;
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				CHECK_CLOSE (1.0f, 1.1f, 0.01f);    line = __LINE__;
			}

			CHECK_EQUAL ("CheckCloseFailureContainsCorrectInfo", reporter.lastFailedTest);
			CHECK_EQUAL (__FILE__, reporter.lastFailedFile);
			CHECK_EQUAL (line, reporter.lastFailedLine);
		}

		UNITTEST_TEST(CheckCloseFailureBecauseOfExceptionIncludesCheckContents)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				CHECK_CLOSE ((float)ThrowingFunction(), 1.0001f, 0.1f);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "(float)ThrowingFunction()"));
			CHECK (gStringFind(reporter.lastFailedMessage, "1.0001f"));
		}

		UNITTEST_TEST(CheckCloseDoesNotHaveSideEffectsWhenPassing)
		{
			g_sideEffect = 0;
			{
				UnitTest::TestResults testResults_;
				CHECK_CLOSE (1, FunctionWithSideEffects(), 0.1f);
			}
			CHECK_EQUAL (1, g_sideEffect);
		}

		UNITTEST_TEST(CheckCloseDoesNotHaveSideEffectsWhenFailing)
		{
			g_sideEffect = 0;
			{
				UnitTest::TestResults testResults_;
				CHECK_CLOSE (2, FunctionWithSideEffects(), 0.1f);
			}
			CHECK_EQUAL (1, g_sideEffect);
		}


		class ThrowingObject
		{
		public:
			float operator[](int) const
			{
				UT_THROW1("Test throw");
			}
		};

		float const* FunctionWithSideEffects2()
		{
			++g_sideEffect;
			static float const data[] = {1,2,3,4};
			return data;
		}


		UNITTEST_TEST(CheckArrayCloseSuceedsOnEqual)
		{
			bool failure = true;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				const float data[4] = { 0, 1, 2, 3 };
				CHECK_ARRAY_CLOSE (data, data, 4, 0.01f);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (!failure);
		}

		UNITTEST_TEST(CheckArrayCloseFailsOnNotEqual)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				int const data1[4] = { 0, 1, 2, 3 };
				int const data2[4] = { 0, 1, 3, 3 };
				CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckArrayCloseFailureIncludesCheckExpectedAndActual)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				int const data1[4] = { 0, 1, 2, 3 };
				int const data2[4] = { 0, 1, 3, 3 };
				CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "xpected [ 0 1 2 3 ]"));
			CHECK (gStringFind(reporter.lastFailedMessage, "was [ 0 1 3 3 ]"));
		}

		UNITTEST_TEST(CheckArrayCloseFailureContainsCorrectInfo)
		{
			int line = 0;
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				int const data1[4] = { 0, 1, 2, 3 };
				int const data2[4] = { 0, 1, 3, 3 };
				CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);     line = __LINE__;
			}

			CHECK_EQUAL ("CheckArrayCloseFailureContainsCorrectInfo", reporter.lastFailedTest);
			CHECK_EQUAL (__FILE__, reporter.lastFailedFile);
			CHECK_EQUAL (line, reporter.lastFailedLine);
		}

		UNITTEST_TEST(CheckArrayCloseFailureIncludesTolerance)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				float const data1[4] = { 0, 1, 2, 3 };
				float const data2[4] = { 0, 1, 3, 3 };
				CHECK_ARRAY_CLOSE (data1, data2, 4, 0.01f);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "0.01"));
		}


		UNITTEST_TEST(CheckArrayCloseFailsOnException)
		{
			bool failure = false;
			{
				RecordingReporter reporter;
				UnitTest::TestResults testResults_(&reporter);
				const float data[4] = { 0, 1, 2, 3 };
				ThrowingObject obj;
				CHECK_ARRAY_CLOSE (data, obj, 3, 0.01f);
				failure = (testResults_.getFailureCount() > 0);
			}

			CHECK (failure);
		}

		UNITTEST_TEST(CheckArrayCloseFailureOnExceptionIncludesCheckContents)
		{
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				const float data[4] = { 0, 1, 2, 3 };
				ThrowingObject obj;
				CHECK_ARRAY_CLOSE (data, obj, 3, 0.01f);
			}

			CHECK (gStringFind(reporter.lastFailedMessage, "data"));
			CHECK (gStringFind(reporter.lastFailedMessage, "obj"));
		}

		UNITTEST_TEST(CheckArrayCloseDoesNotHaveSideEffectsWhenPassing)
		{
			g_sideEffect = 0;
			{
				UnitTest::TestResults testResults_;
				const float data[] = { 0, 1, 2, 3 };
				CHECK_ARRAY_CLOSE (data, FunctionWithSideEffects2(), 4, 0.01f);
			}
			CHECK_EQUAL (1, g_sideEffect);
		}

		UNITTEST_TEST(CheckArrayCloseDoesNotHaveSideEffectsWhenFailing)
		{
			g_sideEffect = 0;
			{
				UnitTest::TestResults testResults_;
				const float data[] = { 0, 1, 3, 3 };
				CHECK_ARRAY_CLOSE (data, FunctionWithSideEffects2(), 4, 0.01f);
			}
			CHECK_EQUAL (1, g_sideEffect);
		}
	}
}
UNITTEST_SUITE_END
