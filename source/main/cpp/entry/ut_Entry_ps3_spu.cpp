#ifdef TARGET_PS3_SPU
#include <new>
#include <stdlib.h>

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestState.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"

#include <spu_printf.h>

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

int gProgramSize = 0;
int gStackSize   = 0;

void getProgramAndStackSizeForSPU(int* programSize, int* stackSize)
{
	*programSize = gProgramSize;
	*stackSize   = gStackSize;
}

int main(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
	gProgramSize = arg1;
	gStackSize = arg2;

	UnitTest::TestState::sCreateInstance();
	UnitTest::TestState::sGetInstance()->setTestState(arg3, arg4);

	spu_printf("arg1(SpuProgramSize)=%d arg2(SpuStackSize)=%d \n", gProgramSize, gStackSize);
	spu_printf("Test Count: %d \t Failure Count: %d\n", arg3, arg4);

	UnitTest::SetAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	UnitTest::TestState::sDestroyInstance();

	return result ? 0 : -1;
}
#endif
