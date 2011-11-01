#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_Stdout.h"

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
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestThreadSuite);
UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestCountingAllocator);

// UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestList);		//*
// UNITTEST_SUITE_DECLARE(xUnitTestUnitTest, TestTestRunner);		//*

#if defined(TARGET_PC)
#include "ut_Allocator_Win32.h"
#elif defined(TARGET_PS3)
#include "ut_Allocator_PS3.h"
#elif defined(TARGET_WII)
#include "ut_Allocator_Wii.h"
#endif


bool	gRunUnitTest(UnitTest::TestReporter& reporter)
{
	UnitTestAllocator * unittestAllocator = gCreateSystemAllocator();
	UnitTest::SetAllocator(unittestAllocator);

	int r = UNITTEST_SUITE_RUN(reporter, xUnitTestUnitTest);

	unittestAllocator->Release();
	UnitTest::SetAllocator(NULL);

	return r == 0;
}
