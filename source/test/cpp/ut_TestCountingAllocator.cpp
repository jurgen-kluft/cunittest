#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_ReportAssert.h"
#include "cunittest/private/ut_Config.h"

UNITTEST_SUITE_BEGIN(TestCountingAllocator)
{
	UNITTEST_FIXTURE(fixture1)
	{
		int*	a = nullptr;
		int*	c = nullptr;
		int*	d = nullptr;

		UNITTEST_FIXTURE_SETUP() 
		{
			d = (int *)FixtureAllocator->Allocate(sizeof(int));
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
			FixtureAllocator->Deallocate(a);
			FixtureAllocator->Deallocate(c);
		}

		UNITTEST_TEST(NoDeallocate1)
		{
			a = (int *)FixtureAllocator->Allocate(sizeof(int));
		}

		UNITTEST_TEST(Correct1)
		{
			int*	b = (int *)FixtureAllocator->Allocate(sizeof(int));
			FixtureAllocator->Deallocate(b);
		}

		UNITTEST_TEST(NoDeallocate2)
		{
			c = (int *)FixtureAllocator->Allocate(sizeof(int));
		}

		UNITTEST_TEST(ExtraDeallocate_Error)
		{
			FixtureAllocator->Deallocate(d);
		}


	}

	UNITTEST_FIXTURE(fixture_with_memory_leak_in_setup_teardown)
	{
		int * d;

		UNITTEST_FIXTURE_SETUP() 
		{
			d = (int *)FixtureAllocator->Allocate(sizeof(int));
		}

		UNITTEST_FIXTURE_TEARDOWN() 
		{
			FixtureAllocator->Deallocate(d);
		}

	}
}
UNITTEST_SUITE_END
