#include "cunittest/cunittest.h"
#include "cunittest/private/ut_AssertException.h"
#include "cunittest/private/ut_ReportAssert.h"

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

			UT_TRY_BEGIN
				UT_THROW1(AssertException("", "",0));
			UT_CATCH(const AssertException&)
				caught = true;
			UT_CATCH_END

			CHECK (true == caught);
		}
		
		UNITTEST_TEST(ReportAssertSetsCorrectInfoInException)
		{
			const int lineNumber = 12345;
			const char* description = "description";
			const char* filename = "filename";

			UT_TRY_BEGIN
				UT_THROW1(AssertException(description, filename, lineNumber));
			UT_CATCH(AssertException const& e)
				CHECK_EQUAL(description, e.what());
				CHECK_EQUAL(filename, e.filename());
				CHECK_EQUAL(lineNumber, e.lineNumber());
			UT_CATCH_END
		}
	}
}
UNITTEST_SUITE_END
