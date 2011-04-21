#ifdef TARGET_WII
#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"

#include <revolution\mem.h>

class UnitTestAllocator : public UnitTest::Allocator
{
public:
	MEMAllocator			mMemAllocator;
	int						mDefaultAlignment;
	int						mNumAllocations;

	UnitTestAllocator()
		: mDefaultAlignment(4)
		, mNumAllocations(0)
	{
		// Heap on MEM2
		void* arenaLo = OSGetMEM2ArenaLo();
		void* arenaHi = OSGetMEM2ArenaHi();

		MEMHeapHandle heapHandle = MEMCreateExpHeap(arenaLo, (u32)arenaHi - (u32)arenaLo);
		if ( heapHandle != MEM_HEAP_INVALID_HANDLE )
		{
			OSSetMEM2ArenaLo(arenaHi);
			MEMInitAllocatorForExpHeap(&mMemAllocator, heapHandle, mDefaultAlignment);
		}
	}

	void*	Allocate(int size)
	{
		++mNumAllocations;
		void* mem = MEMAllocFromAllocator(&mMemAllocator, size);
		return mem;
	}
	void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		MEMFreeToAllocator(&mMemAllocator, ptr);
	}

	void	Release()
	{
	}
};

class UnitTestObserver : public UnitTest::Observer
{
public:
	void	BeginFixture(const char* filename, const char* suite_name, const char* fixture_name)
	{
	}
	void	EndFixture()
	{
	}
};


extern bool gRunUnitTest(UnitTest::TestReporter& reporter);
void main(int argc, char** argv)
{
	UnitTestAllocator unittestAllocator;
	UnitTest::SetAllocator(&unittestAllocator);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	if (unittestAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, __FUNCTION__, "memory leaks detected!");
		result = false;
	}

	//return result ? 0 : -1;
}



#endif
