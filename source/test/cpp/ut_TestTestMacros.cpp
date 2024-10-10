#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TestMacros.h"
#include "cunittest/private/ut_TestResults.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/ut_RecordingReporter.h"

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

	}
}
UNITTEST_SUITE_END
