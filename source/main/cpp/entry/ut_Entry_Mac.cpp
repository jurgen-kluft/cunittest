#ifdef TARGET_MAC
#    include <stdlib.h>

#    include "cunittest/cunittest.h"
#    include "cunittest/private/ut_Config.h"
#    include "cunittest/private/ut_TimeHelpers.h"
#    include "cunittest/private/ut_TestReporterStdout.h"
#    include "cunittest/private/ut_TestReporterTeamCity.h"

extern bool gRunUnitTest(UnitTest::TestReporter& reporter, UnitTest::TestContext& context);

int main(int argc, char** argv)
{
    UnitTest::MemCheckAllocator  memcheck_allocator;
    UnitTest::NullObserver       observer;
    UnitTest::TestReporterStdout stdout_reporter;
    UnitTest::TestReporter&      reporter  = stdout_reporter;
    UnitTest::TestAllocator*     allocator = &memcheck_allocator;

    UnitTest::TestContext context;
    context.mAllocator = allocator;
    context.mObserver  = &observer;

    // TODO parse the names of the test suites, fixtures, and tests that should be run:
    // --run=Suite1,Suite2/Fixture1,Suite3/Fixture2/Test1

    UnitTest::g_InitTimer();

    bool result = gRunUnitTest(reporter, context);

    return result ? 0 : -1;
}

#endif
