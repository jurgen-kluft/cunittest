#include "..\UnitCore\ut_UnitTest.h"
#include "..\UnitCore\Implementation\ut_TestReporterStdout.h"
#include "..\UnitCore\Implementation\ut_TestReporterTeamCity.h"

int main(int, char const *[])
{
	UnitTest::TestReporterStdout reporter;
    return UnitTest::runAllTests(reporter);
}
