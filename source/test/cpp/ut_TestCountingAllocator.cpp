#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_ReportAssert.h"
#include "xunittest\private\ut_Config.h"

UNITTEST_SUITE_BEGIN(TestCountingAllocator)
{
	UNITTEST_FIXTURE(fixture1)
	{
		UNITTEST_FIXTURE_SETUP() {}

		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(NoDeallocate1)
		{
			int * a = (int *)UnitTest::GetAllocator()->Allocate(sizeof(int));
		}

		UNITTEST_TEST(Correct1)
		{
			int * a = (int *)UnitTest::GetAllocator()->Allocate(sizeof(int));
			UnitTest::GetAllocator()->Deallocate(a);
		}

		UNITTEST_TEST(NoDeallocate2)
		{
			int * a = (int *)UnitTest::GetAllocator()->Allocate(sizeof(int));
		}


	}

	UNITTEST_FIXTURE(fixture_with_memory_leak_in_setup_teardown)
	{
		int * a;

		UNITTEST_FIXTURE_SETUP() 
		{
			a = (int *)UnitTest::GetAllocator()->Allocate(sizeof(int));
		}

		UNITTEST_FIXTURE_TEARDOWN() 
		{
			/// Oops forgetting to delete 'a'
		}

	}
}
UNITTEST_SUITE_END