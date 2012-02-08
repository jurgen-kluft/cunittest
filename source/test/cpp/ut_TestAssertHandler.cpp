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

			_TRY_BEGIN
				_THROW1(AssertException("", "",0));
			_CATCH(const AssertException& e)
				caught = true;
			_CATCH_END

			CHECK (true == caught);
		}
		
#ifndef TARGET_PS3_SPU

		UNITTEST_TEST(ReportAssertSetsCorrectInfoInException)
		{
			const int lineNumber = 12345;
			const char* description = "description";
			const char* filename = "filename";

			_TRY_BEGIN
				_THROW1(AssertException(description, filename, lineNumber));
			_CATCH(AssertException const& e)
				CHECK_EQUAL(description, e.what());
				CHECK_EQUAL(filename, e.filename());
				CHECK_EQUAL(lineNumber, e.lineNumber());
			_CATCH_END
		}
#endif //TARGET_PS3_SPU		
	}
}
UNITTEST_SUITE_END
