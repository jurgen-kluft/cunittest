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


#ifdef TARGET_PC
	static int sNumAllocations = 0;
	void*	UnitTest::Allocate(int size)
	{
		++sNumAllocations;
		return malloc(size + 10000);
	}
	void	UnitTest::Deallocate(void* ptr)
	{
		--sNumAllocations;
		free(ptr);
	}
#endif


int main(int, char const *[])
{
	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	int r = UNITTEST_SUITE_RUN(reporter, xUnitTestUnitTest);
	if (sNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, __FUNCTION__, "memory leaks detected!");
		r = -1;
	}

	return r;
}
