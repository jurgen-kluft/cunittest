#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TimeConstraint.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TimeHelpers.h"
#include "xunittest\ut_RecordingReporter.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestTimeConstraint)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(TimeConstraintSucceedsWithFastTest)
		{
			TestResults result;
			{
				TimeConstraint t(200, result, "", 0, "");
				TimeHelpers::sleepMs(5);
			}
			CHECK_EQUAL (0, result.getFailureCount());
		}

		UNITTEST_TEST(TimeConstraintFailsWithSlowTest)
		{
			TestResults result;
			{
				TimeConstraint t(10, result, "", 0, "");
				TimeHelpers::sleepMs(20);
			}
			CHECK_EQUAL (1, result.getFailureCount());
		}

		UNITTEST_TEST(TimeConstraintFailureIncludesCorrectData)
		{
			RecordingReporter reporter;
			TestResults result(&reporter);
			{
				TimeConstraint t(10, result, "filename", 123, "testname");
				TimeHelpers::sleepMs(20);
			}
			CHECK (gStringFind(reporter.lastFailedFile, "filename"));
			CHECK_EQUAL (123, reporter.lastFailedLine);
			CHECK (gStringFind(reporter.lastFailedTest, "testname"));
		}

		UNITTEST_TEST(TimeConstraintFailureIncludesTimeoutInformation)
		{
			RecordingReporter reporter;
			TestResults result(&reporter);
			{
				TimeConstraint t(10, result, "", 0, "");
				TimeHelpers::sleepMs(20);
			}
			CHECK (gStringFind(reporter.lastFailedMessage, "ime constraint"));
			CHECK (gStringFind(reporter.lastFailedMessage, "under 10ms"));
		}

		UNITTEST_TEST(TimeConstraintMacroUsesCorrectInfo)
		{
			int testLine = 0;
			RecordingReporter reporter;
			{
				UnitTest::TestResults testResults_(&reporter);
				UNITTEST_TIME_CONSTRAINT(10);                    testLine = __LINE__;
				TimeHelpers::sleepMs(20);
			}
			CHECK_EQUAL (1, reporter.testFailedCount);
			CHECK (gStringFind(reporter.lastFailedFile, __FILE__));
			CHECK_EQUAL (testLine, reporter.lastFailedLine);
			CHECK (gStringFind(reporter.lastFailedTest, "TimeConstraintMacroUsesCorrectInfo"));
		}

	}
}
UNITTEST_SUITE_END
