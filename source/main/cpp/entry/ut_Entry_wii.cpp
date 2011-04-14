#ifdef TARGET_WII

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"

class UnitTestAllocator : public UnitTest::Allocator
{
public:
	int		mNumAllocations;

	UnitTestAllocator()
		: mNumAllocations(0)
	{
	}

	void*	Allocate(int size)
	{
		++mNumAllocations;
		return malloc(size);
	}
	void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		free(ptr);
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