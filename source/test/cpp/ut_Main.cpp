#ifdef TARGET_N3DS
#include <nn.h>
#include <nn\os.h>
#endif

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"

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

class UnitTestAllocator : public UnitTest::Allocator
{
public:
	int		mNumAllocations;
	#ifdef TARGET_N3DS
		nn::fnd::ExpHeap mAppHeap;
	#endif

	UnitTestAllocator()
		: mNumAllocations(0)
	{
	#ifdef TARGET_N3DS
		mAppHeap.Initialize(nn::os::GetDeviceMemoryAddress(), 8 * 1024 * 1024, nn::os::ALLOCATE_OPTION_LINEAR);
	#endif
	}

	void*	Allocate(int size)
	{
		++mNumAllocations;
	#ifdef TARGET_PC
		return malloc(size);
	#elif defined(TARGET_N3DS)
		void* mem = mAppHeap.Allocate(size+32);
		return mem;
	#else
		return 0;
	#endif
	}
	void	Deallocate(void* ptr)
	{
		--mNumAllocations;
	#ifdef TARGET_PC
		free(ptr);
	#elif defined(TARGET_N3DS)
		if (ptr!=0)
			mAppHeap.Free(ptr);
	#endif
	}

	void	Release()
	{
	#ifdef TARGET_N3DS
		mAppHeap.Finalize(); 
	#endif
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

int runUnitTest()
{
	UnitTestAllocator unittestAllocator;
	UnitTestObserver unittestObserver;
	UnitTest::SetAllocator(&unittestAllocator);
	UnitTest::SetObserver(&unittestObserver);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	int r = UNITTEST_SUITE_RUN(reporter, xUnitTestUnitTest);
	if (unittestAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, __FUNCTION__, "memory leaks detected!");
		r = -1;
	}

	unittestAllocator.Release();
	return r;
}

#ifdef TARGET_N3DS
void nnMain( void )
{
    nn::os::Initialize();
    nn::fs::Initialize();

	runUnitTest();
}
#endif

#ifdef TARGET_PC
int main(int, char const *[])
{
	return runUnitTest();
}
#endif