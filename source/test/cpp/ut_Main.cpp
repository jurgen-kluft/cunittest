#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TestReporter.h"
#include "cunittest/private/ut_Stdout.h"
#include "cunittest/private/ut_Config.h"

#include <stdlib.h>

UNITTEST_SUITE_LIST

bool gRunUnitTest(UnitTest::TestReporter& reporter, UnitTest::TestContext& context)
{
    int r = UNITTEST_SUITE_RUN(context, reporter, cUnitTest);

    return r == 0;
}
