#ifdef TARGET_PS3_SPU
#include <new>
#include <stdlib.h>

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"

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

#include <spu_printf.h>

extern bool gRunUnitTest(UnitTest::TestReporter& reporter);

int gProgramSize = 0;
int gStackSize   = 0;

// The indices start at 1 instead of 0; 0 means no exception has ever happened yet
unsigned int exceptionSuiteIndex;
unsigned int exceptionFixtureIndex;
unsigned int exceptionTestIndex;
unsigned int failureCount;

void getProgramAndStackSizeForSPU(int* programSize, int* stackSize)
{
	*programSize = gProgramSize;
	*stackSize   = gStackSize;
}

int main(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
	gProgramSize = arg1;
	gStackSize = arg2;

	exceptionSuiteIndex = arg3 >> 16;
	exceptionFixtureIndex = arg3 & 0x0000FFFF;
	exceptionTestIndex = arg4 >> 16;
	failureCount = arg4 & 0x0000FFFF;

	spu_printf("arg1(SpuProgramSize)=%d arg2(SpuStackSize)=%d \n", gProgramSize, gStackSize);
	spu_printf("Suite: %d \t Fixture: %d \t Test: %d \t Failure Count: %d\n", exceptionSuiteIndex, exceptionFixtureIndex, exceptionTestIndex, failureCount);

	UnitTest::SetAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	return result ? 0 : -1;
}
#endif
