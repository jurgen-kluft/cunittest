#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\ut_RecordingReporter.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestTestResults)
{
	UNITTEST_FIXTURE(fixture1)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(StartsWithNoTestsRun)
		{
			TestResults results;
			CHECK_EQUAL (0, results.getTestCount());
		}

		UNITTEST_TEST(RecordsNumbersOfTests)
		{
			TestResults results;
			results.onTestStart("testname1");
			results.onTestStart("testname2");
			results.onTestStart("testname3");
			CHECK_EQUAL(3, results.getTestCount());
		}

		UNITTEST_TEST(StartsWithNoTestsFailing)
		{
			TestResults results;
			CHECK_EQUAL (0, results.getFailureCount());
		}

		UNITTEST_TEST(RecordsNumberOfFailures)
		{
			TestResults results;
			results.onTestFailure("nothing", 0, "", "expected failure");
			results.onTestFailure("nothing", 0, "", "expected failure");
			CHECK_EQUAL(2, results.getFailureCount());
		}

		UNITTEST_TEST(NotifiesRecorderOfTestStartWithCorrectInfo)
		{
			RecordingReporter reporter;
			TestResults results(&reporter);
			results.onTestStart("mytest");
			CHECK_EQUAL (1, reporter.testRunCount);
			CHECK_EQUAL ("mytest", reporter.lastStartedTest);
		}

		UNITTEST_TEST(NotifiesRecorderOfTestFailureWithCorrectInfo)
		{
			RecordingReporter reporter;
			TestResults results(&reporter);
			results.onTestFailure("filename", 123, "testname", "failurestring");
			CHECK_EQUAL (1, reporter.testFailedCount);
			CHECK_EQUAL ("filename", reporter.lastFailedFile);
			CHECK_EQUAL (123, reporter.lastFailedLine);
			CHECK_EQUAL ("testname", reporter.lastFailedTest);
			CHECK_EQUAL ("failurestring", reporter.lastFailedMessage);
		}

		UNITTEST_TEST(NotifiesRecorderOfTestEnd)
		{
			RecordingReporter reporter;
			TestResults results(&reporter);
			results.onTestEnd("mytest", 0.1234f);
			CHECK_EQUAL (1, reporter.testFinishedCount);
			CHECK_EQUAL ("mytest", reporter.lastFinishedTest);
			CHECK_CLOSE (0.1234f, reporter.lastFinishedTestTime, 0.0001f);
		}
	}
}
UNITTEST_SUITE_END