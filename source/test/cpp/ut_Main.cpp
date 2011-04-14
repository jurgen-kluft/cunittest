#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporter.h"

UNITTEST_SUITE_LIST(xUnitTestUnitTest);

UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestAssertHandler);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestStringBuilder);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestCpp);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestCheckMacros);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestChecks);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTest);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestMacros);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestResults);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTimeConstraint);
//UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestList);
//UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestRunner);


bool	gRunUnitTest(UnitTest::TestReporter& reporter)
{
	int r = UNITTEST_SUITE_RUN(reporter, xUnitTestUnitTest);
	return r == 0;
}
