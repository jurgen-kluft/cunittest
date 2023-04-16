#ifdef TARGET_PC
#include <windows.h>

#include "cunittest/cunittest.h"
#include "cunittest/private/ut_TimeHelpers.h"
#include "cunittest/private/ut_TestReporterStdout.h"
#include "cunittest/private/ut_TestReporterTeamCity.h"

class UnitTestObserver : public UnitTest::TestObserver
{
public:
    virtual void BeginSuite(const char* filename, const char* suite_name) {}
    virtual void EndSuite() {}

    virtual void BeginFixture(const char* filename, const char* suite_name, const char* fixture_name) {}
    virtual void EndFixture() {}

    virtual void BeginTest(const char* filename, const char* suite_name, const char* fixture_name, const char* test_name) {}
    virtual void EndTest() {}
};

class UnitTestDummyAllocator : public UnitTest::TestAllocator
{
public:
	UnitTestDummyAllocator() {}

	virtual void*  Allocate(unsigned int size, unsigned int alignment) { return nullptr; }
	virtual unsigned int Deallocate(void* ptr) { return 0; }
};

extern bool gRunUnitTest(UnitTest::TestReporter& reporter, UnitTest::TestContext& context);

int main(int argc, char** argv)
{
    UnitTestObserver observer;

    UnitTest::TestReporterStdout stdout_reporter;
    UnitTest::TestReporter&      reporter = stdout_reporter;

    UnitTest::TestContext context;
	context.mAllocator = &dummy_allocator;
	context.mObserver = &observer;

    UnitTest::g_InitTimer();

    bool result = gRunUnitTest(reporter, context);

    return result ? 0 : -1;
}


#endif