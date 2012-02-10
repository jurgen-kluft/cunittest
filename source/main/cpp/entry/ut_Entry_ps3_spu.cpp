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

void getProgramAndStackSizeForSPU(int* programSize, int* stackSize)
{
	*programSize = gProgramSize;
	*stackSize   = gStackSize;
}



int main(int arg1, int arg2)
{
	gProgramSize = arg1;
	gStackSize = arg2;

	spu_printf("arg1(SpuProgramSize)=%d arg2(SpuStackSize)=%d \n", arg1, arg2);

	UnitTest::SetAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	return result ? 0 : -1;
}
#endif
