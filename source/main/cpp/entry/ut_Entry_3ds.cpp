#ifdef TARGET_N3DS

#ifdef TARGET_N3DS
#include <nn.h>
#include <nn\os.h>
#endif

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"


class UnitTestAllocator : public UnitTest::Allocator
{
public:
	int		mNumAllocations;
	nn::fnd::ExpHeap mAppHeap;

	UnitTestAllocator()
		: mNumAllocations(0)
	{
		mAppHeap.Initialize(nn::os::GetDeviceMemoryAddress(), 8 * 1024 * 1024, nn::os::ALLOCATE_OPTION_LINEAR);
	}

	void*	Allocate(int size)
	{
		++mNumAllocations;
		void* mem = mAppHeap.Allocate(size+32);
		return mem;
	}
	void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		if (ptr!=0)
			mAppHeap.Free(ptr);
	}

	void	Release()
	{
		mAppHeap.Finalize(); 
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
void nnMain( void )
{
	nn::os::Initialize();
    nn::fs::Initialize();

	UnitTestAllocator unittestAllocator;
	UnitTest::SetAllocator(&unittestAllocator);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;
    	
	gRunUnitTest(reporter);

	if (unittestAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, __FUNCTION__, "memory leaks detected!");
		r = -1;
	}

	unittestAllocator.Release();
}

#endif