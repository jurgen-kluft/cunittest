#include "xunittest/xunittest.h"
#include "xunittest/private/ut_TestReporter.h"
#include "xunittest/private/ut_ReportAssert.h"
#include "xunittest/private/ut_Config.h"

UNITTEST_SUITE_BEGIN(TestCountingAllocator)
{
	UNITTEST_FIXTURE(fixture1)
	{
		int*	a = nullptr;
		int*	c = nullptr;
		int*	d = nullptr;

		UNITTEST_FIXTURE_SETUP() 
		{
			d = (int *)UnitTest::__private::GetAllocator()->Allocate(sizeof(int));
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
			UnitTest::__private::GetAllocator()->Deallocate(a);
			UnitTest::__private::GetAllocator()->Deallocate(c);
		}

		UNITTEST_TEST(NoDeallocate1)
		{
			a = (int *)UnitTest::__private::GetAllocator()->Allocate(sizeof(int));
		}

		UNITTEST_TEST(Correct1)
		{
			int*	b = (int *)UnitTest::__private::GetAllocator()->Allocate(sizeof(int));
			UnitTest::__private::GetAllocator()->Deallocate(b);
		}

		UNITTEST_TEST(NoDeallocate2)
		{
			c = (int *)UnitTest::__private::GetAllocator()->Allocate(sizeof(int));
		}

		UNITTEST_TEST(ExtraDeallocate_Error)
		{
			UnitTest::__private::GetAllocator()->Deallocate(d);
		}


	}

	UNITTEST_FIXTURE(fixture_with_memory_leak_in_setup_teardown)
	{
		int * d;

		UNITTEST_FIXTURE_SETUP() 
		{
			d = (int *)UnitTest::__private::GetAllocator()->Allocate(sizeof(int));
		}

		UNITTEST_FIXTURE_TEARDOWN() 
		{
			UnitTest::__private::GetAllocator()->Deallocate(d);
		}

	}
}
UNITTEST_SUITE_END
