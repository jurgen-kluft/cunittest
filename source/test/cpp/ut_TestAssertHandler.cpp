#include "xunittest\xunittest.h"
#include "xunittest\private\ut_AssertException.h"
#include "xunittest\private\ut_ReportAssert.h"

using namespace UnitTest;

UNITTEST_SUITE_BEGIN(TestAssertHandler)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(ReportAssertThrowsAssertException)
		{
			bool caught = false;

			try
			{
				throw AssertException("", "",0);;
			}
			catch(const AssertException& e)
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
				throw AssertException(description, filename, lineNumber);
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
