#ifdef TARGET_PS3
#include <new>
#include <stdlib.h>

#include <cell/sysmodule.h>

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

extern bool gRunUnitTest(UnitTest::TestReporter& reporter);
int main(int argc, char** argv)
{
	//cellSysmoduleLoadModule(CELL_SYSMODULE_USBD);
	//cellSysmoduleLoadModule(CELL_SYSMODULE_NET);
	cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
	cellSysmoduleLoadModule(CELL_SYSMODULE_IO);
	//cellSysmoduleLoadModule(CELL_SYSMODULE_AUDIO);
	cellSysmoduleLoadModule(CELL_SYSMODULE_RESC);
	cellSysmoduleLoadModule(CELL_SYSMODULE_RTC);
	cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_SAVEDATA);
	cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_USERINFO);
	cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_GAME);
	//cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP_TROPHY);

	UnitTest::SetCountingAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	return result ? 0 : -1;
}
#endif
