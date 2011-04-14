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

	virtual void*	Allocate(int size)
	{
		++mNumAllocations;
		void* mem = mAppHeap.Allocate(size+32);
		return mem;
	}
	virtual void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		if (ptr!=0)
			mAppHeap.Free(ptr);
	}

	virtual void	Release()
	{
		mAppHeap.Finalize(); 
	}
};


class UnitTestObserver : public UnitTest::Observer
{
public:
	virtual void	BeginFixture(const char* filename, const char* suite_name, const char* fixture_name)
	{
	}
	virtual void	EndFixture()
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
    	
	bool result = gRunUnitTest(reporter);

	if (unittestAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, __FUNCTION__, "memory leaks detected!");
		result = false;
	}

	unittestAllocator.Release();
	//return result ? 0 : -1;
}

#endif
