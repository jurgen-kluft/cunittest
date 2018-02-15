#include "xunittest/xunittest.h"
#include "xunittest/ut_RecordingReporter.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestChecks)
{
	UNITTEST_FIXTURE(fixture)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		char txt1[] = "Hello"; // non-const on purpose so no folding of duplicate data
		char txt2[] = "Hello";

		UNITTEST_TEST(checkEqualsWithStringsFailsOnDifferentStrings)
		{
			char txt1[] = "Hello";
			char txt2[] = "Hallo";
			TestResults results;
			checkEqual(results, txt1, txt2, "", "", 0);
			CHECK_EQUAL (1, results.getFailureCount());
		}

		UNITTEST_TEST(checkEqualsWithStringsWorksOnContentsNonConstNonConst)
		{
			char const* const p1 = txt1;
			char const* const p2 = txt2;
			TestResults results;
			checkEqual(results, p1, p2, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkEqualsWithStringsWorksOnContentsConstConst)
		{
			char* const p1 = txt1;
			char* const p2 = txt2;
			TestResults results;
			checkEqual(results, p1, p2, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkEqualsWithStringsWorksOnContentsNonConstConst)
		{
			char* const p1 = txt1;
			char const* const p2 = txt2;
			TestResults results;
			checkEqual(results, p1, p2, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkEqualsWithStringsWorksOnContentsConstNonConst)
		{
			char const* const p1 = txt1;
			char* const p2 = txt2;
			TestResults results;
			checkEqual(results, p1, p2, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkEqualsWithStringsWorksOnContentsWithALiteral)
		{
			char const* const p1 = txt1;
			TestResults results;
			checkEqual(results, "Hello", p1, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkEqualFailureIncludesCheckExpectedAndActual)
		{
			RecordingReporter reporter;
			TestResults results(&reporter);
			const int something = 2;
			checkEqual (results, 1, something, "", "", 0);

			CHECK (gStringFind(reporter.lastFailedMessage, "xpected 1"));
			CHECK (gStringFind(reporter.lastFailedMessage, "was 2"));
		}


		UNITTEST_TEST(checkCloseTrue)
		{
			TestResults results;
			checkClose(results, 3.001f, 3.0f, 0.1f, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkCloseFalse)
		{
			TestResults results;
			checkClose(results, 3.12f, 3.0f, 0.1f, "", "", 0);
			CHECK_EQUAL (1, results.getFailureCount());
		}

		UNITTEST_TEST(checkCloseWithZeroEpsilonWorksForSameNumber)
		{
			TestResults results;
			checkClose(results, 0.1f, 0.1f, 0, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkCloseWithNaNFails)
		{
			union
			{
				unsigned int bitpattern;
				float nan;
			};
			bitpattern = 0xFFFFFFFF;
			TestResults results;
			checkClose(results, 3.0f, nan, 0.1f, "", "", 0);
			CHECK_EQUAL (1, results.getFailureCount());
		}

		UNITTEST_TEST(checkCloseWithNaNAgainstItselfFails)
		{
			union
			{
				unsigned int bitpattern;
				float nan;
			};
			bitpattern = 0xFFFFFFFF;
			TestResults results;
			checkClose(results, nan, nan, 0.1f, "", "", 0);
			CHECK_EQUAL (1, results.getFailureCount());
		}

		UNITTEST_TEST(checkCloseFailureIncludesCheckExpectedAndActual)
		{
			RecordingReporter reporter;
			TestResults results(&reporter);
			const float expected = 0.9f;
			const float actual = 1.1f;
			checkClose (results, expected, actual, 0.01f, "", "", 0);

			CHECK (gStringFind(reporter.lastFailedMessage, "xpected 0.9"));
			CHECK (gStringFind(reporter.lastFailedMessage, "was 1.1"));
		}

		UNITTEST_TEST(checkCloseFailureIncludesTolerance)
		{
			RecordingReporter reporter;
			TestResults results(&reporter);
			checkClose (results, 2, 3, 0.01f, "", "", 0);

			CHECK (gStringFind(reporter.lastFailedMessage, "0.01"));
		}



		UNITTEST_TEST(checkArrayEqualTrue)
		{
			TestResults results;
			int const array[3] = { 1, 2, 3 };
			checkArrayEqual(results, array, array, 3, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(checkArrayEqualFalse)
		{
			TestResults results;
			int const array1[3] = { 1, 2, 3 };
			int const array2[3] = { 1, 2, 2 };
			checkArrayEqual(results, array1, array2, 3, "", "", 0);
			CHECK_EQUAL (1, results.getFailureCount());
		}

		UNITTEST_TEST(CheckArrayCloseTrue)
		{
			TestResults results;
			float const array1[3] = { 1.0f, 1.5f, 2.0f };
			float const array2[3] = { 1.01f, 1.51f, 2.01f };
			checkArrayClose(results, array1, array2, 3, 0.02f, "", "", 0);
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(CheckArrayCloseFalse)
		{
			TestResults results;
			float const array1[3] = { 1.0f, 1.5f, 2.0f };
			float const array2[3] = { 1.01f, 1.51f, 2.01f };
			checkArrayClose(results, array1, array2, 3, 0.001f, "", "", 0);
			CHECK_EQUAL (1, results.getFailureCount());
		}
	}
}
UNITTEST_SUITE_END
