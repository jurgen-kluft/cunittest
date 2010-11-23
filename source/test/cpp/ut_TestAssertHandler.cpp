#include "ut_UnitTest.h"
#include "ut_AssertException.h"
#include "ut_ReportAssert.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN_PARTIAL(UnitTestTest)
{
	UNITTEST_FIXTURE(TestAssertHandler)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(ReportAssertThrowsAssertException)
		{
			bool caught = false;

			try
			{
				reportAssert("", "", 0);
			}
			catch(AssertException const&)
			{
				caught = true;
			}

			CHECK (true == caught);
		}

		UNITTEST_TEST(ReportAssertSetsCorrectInfoInException)
		{
			const int lineNumber = 12345;
			const char* description = "description";
			const char* filename = "filename";

			try
			{
				reportAssert(description, filename, lineNumber);
			}
			catch(AssertException const& e)
			{
				CHECK_EQUAL(description, e.what());
				CHECK_EQUAL(filename, e.filename());
				CHECK_EQUAL(lineNumber, e.lineNumber());
			}
		}
	}
}
UNITTEST_SUITE_END

