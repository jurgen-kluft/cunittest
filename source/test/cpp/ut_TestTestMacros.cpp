#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestMacros.h"
#include "xunittest\private\ut_TestList.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\ut_RecordingReporter.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestMacros)
{
	UNITTEST_FIXTURE(part1)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(TestsAreAddedToTheListThroughMacro)
		{
// 			CHECK (gGetTestList()->getHead() != 0);
// 			CHECK (gGetTestList()->getHead()->mNext == 0);
		}
	}

	UNITTEST_FIXTURE(part2)
	{
		UNITTEST_FIXTURE_SETUP()
		{
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}


		UNITTEST_FIXTURE_EX(ThrowingThingie)
		UNITTEST_FIXTURE_EX_OPEN(ThrowingThingie)
		{
			UNITTEST_FIXTURE_SETUP()
			{
				throw "Oops";
			}

			UNITTEST_FIXTURE_TEARDOWN()
			{
				throw "Oops";
			}
		}

		UNITTEST_TEST(ExceptionsInFixtureAreReportedAsHappeningInTheFixture)
		{
			RecordingReporter reporter;
			TestResults result(&reporter);
			nsTestFixtureThrowingThingie::gFixtureInstance.run(result, 0);

			CHECK (gStringFind(reporter.lastFailedMessage, "xception"));
			CHECK (gStringFind(reporter.lastFailedMessage, "fixture"));
			CHECK (gStringFind(reporter.lastFailedMessage, "ThrowingThingie"));
		}
	}
}
UNITTEST_SUITE_END
