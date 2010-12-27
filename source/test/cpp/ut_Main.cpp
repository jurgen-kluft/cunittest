#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"


UNITTEST_SUITE_LIST(xUnitTestUnitTest);

UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestAssertHandler);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestCheckMacros);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestChecks);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestStringBuilder);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTest);
//UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestList);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestMacros);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestResults);
//UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestRunner);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTimeConstraint);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestCpp);


int main(int, char const *[])
{
	UnitTest::TestReporterStdout reporter;
	int r = UNITTEST_SUITE_RUN(reporter, xUnitTestUnitTest);
	return r;
}
