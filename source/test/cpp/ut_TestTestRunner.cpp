#include "xunittest/xunittest.h"
#include "xunittest/ut_RecordingReporter.h"
#include "xunittest/private/ut_ReportAssert.h"
#include "xunittest/private/ut_TestList.h"
#include "xunittest/private/ut_TimeHelpers.h"
#include "xunittest/private/ut_TimeConstraint.h"

using namespace UnitTest;

/*
UNITTEST_SUITE_BEGIN(TestTestRunner)
{
	struct MockTest : public Test
	{
		MockTest(char const* testName, bool const success_, bool const assert_)
			: Test(testName)
			, success(success_)
			, asserted(assert_)
		{
		}

		virtual void runImpl(TestResults& testResults_) const
		{
			if (asserted)
				reportAssert("desc", "file", 0);
			else if (!success)
				testResults_.onTestFailure("filename", 0, "", "message");
		}

		bool success;
		bool asserted;
	};

	class SlowTest : public Test
	{
	public:
		SlowTest() : Test("slow", "filename", 123) {}
		virtual void runImpl(TestResults&) const
		{
			TimeHelpers::sleepMs(20);
		}
	};

	UNITTEST_FIXTURE(fixture1)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(TestRunCount)
		{
			TestList list;
			MockTest test("goodtest", true, false);
			list.add(&test);

			runAllTests(reporter, &list);
			CHECK_EQUAL(1, reporter.testRunCount);
			CHECK_EQUAL("goodtest", reporter.lastStartedTest);
		}
	}

	UNITTEST_FIXTURE(fixture2)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;
		UNITTEST_TEST(TestFinishedCount)
		{
			TestList list;
			MockTest test("goodtest", true, false);
			list.add(&test);

			runAllTests(reporter, &list);
			CHECK_EQUAL(1, reporter.testFinishedCount);
			CHECK_EQUAL("goodtest", reporter.lastFinishedTest);
		}
	}

	UNITTEST_FIXTURE(fixture3)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(TestFinishIsCalledWithCorrectTime)
		{
			TestList list;
			SlowTest test;
			list.add(&test);

			runAllTests(reporter, &list);
			CHECK (reporter.lastFinishedTestTime >= 0.005f && reporter.lastFinishedTestTime <= 0.050f);
		}
	}

	UNITTEST_FIXTURE(fixture4)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(FailureCountIsZeroWhenNoTestsAreRun)
		{
			TestList list;
			CHECK_EQUAL(0, runAllTests(reporter, &list));
			CHECK_EQUAL(0, reporter.testRunCount);
			CHECK_EQUAL(0, reporter.testFailedCount);
		}
	}

	UNITTEST_FIXTURE(fixture5)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(CallsReportFailureOncePerFailingTest)
		{
			TestList list;
			MockTest test1("test", false, false);
			list.add(&test1);
			MockTest test2("test", true, false);
			list.add(&test2);
			MockTest test3("test", false, false);
			list.add(&test3);

			CHECK_EQUAL(2, runAllTests(reporter, &list));
			CHECK_EQUAL(2, reporter.testFailedCount);
		}
	}

	UNITTEST_FIXTURE(fixture6)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(TestsThatAssertAreReportedAsFailing)
		{
			TestList list;
			MockTest test("test", true, true);
			list.add(&test);

			runAllTests(reporter, &list);
			CHECK_EQUAL(1, reporter.testFailedCount);
		}
	}

	UNITTEST_FIXTURE(fixture7)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(FinishedTestsReportDone)
		{
			TestList list;
			MockTest test1("test", true, false);
			MockTest test2("test", false, false);
			list.add(&test1);
			list.add(&test2);

			runAllTests(reporter, &list);
			CHECK_EQUAL(2, reporter.summaryTestCount);
			CHECK_EQUAL(1, reporter.summaryFailureCount);
		}
	}

	UNITTEST_FIXTURE(fixture8)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(SlowTestPassesForHighTimeThreshold)
		{
			TestList list;
			SlowTest test;
			list.add(&test);
			runAllTests(reporter, &list);
			CHECK_EQUAL (0, reporter.testFailedCount);
		}
	}

	UNITTEST_FIXTURE(fixture9)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(SlowTestFailsForLowTimeThreshold)
		{
			TestList list;
			SlowTest test;
			list.add(&test);
			runAllTests(reporter, &list, 3);
			CHECK_EQUAL (1, reporter.testFailedCount);
		}
	}

	UNITTEST_FIXTURE(fixture10)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(SlowTestHasCorrectFailureInformation)
		{
			TestList list;
			SlowTest test;
			list.add(&test);
			runAllTests(reporter, &list, 3);
			CHECK_EQUAL (test.mTestName, reporter.lastFailedTest);
			CHECK (gStringFind(test.mFilename, reporter.lastFailedFile));
			CHECK_EQUAL (test.mLineNumber, reporter.lastFailedLine);
			CHECK (gStringFind(reporter.lastFailedMessage, "Global time constraint failed"));
			CHECK (gStringFind(reporter.lastFailedMessage, "3ms"));
		}
	}

	UNITTEST_FIXTURE(fixture11)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		static RecordingReporter reporter;

		UNITTEST_TEST(SlowTestWithTimeExemptionPasses)
		{
			TestList list;
			class SlowExemptedTest : public Test
			{
			public:
				SlowExemptedTest() : Test("slowexempted", "", 0) {}
				virtual void runImpl(TestResults&) const
				{
					UNITTEST_TIME_CONSTRAINT_EXEMPT();
					TimeHelpers::sleepMs(20);
				}
			};

			SlowExemptedTest test;
			list.add(&test);
			runAllTests(reporter, &list, 3);
			CHECK_EQUAL (0, reporter.testFailedCount);
		}
	}
}
UNITTEST_SUITE_END
*/
