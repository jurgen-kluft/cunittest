#ifdef TARGET_MAC
#include <stdlib.h>

#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TestReporterStdout.h"
#include "cunittest/private/ut_TestReporterTeamCity.h"

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
	UnitTest::SetAllocator(nullptr);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	return result ? 0 : -1;
}



#endif