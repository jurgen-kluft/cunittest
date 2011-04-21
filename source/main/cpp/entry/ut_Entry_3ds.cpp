#ifdef TARGET_N3DS

#include <nn.h>
#include <nn\os.h>

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"


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

	UnitTest::SetAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;
    	
	bool result = gRunUnitTest(reporter);

	//return result ? 0 : -1;
}

#endif
