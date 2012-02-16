#ifdef TARGET_PS3_SPU

#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TestReporter.h"

#include <spu_printf.h>

extern unsigned int exceptionSuiteIndex;
extern unsigned int exceptionFixtureIndex;
extern unsigned int exceptionTestIndex;
extern unsigned int failureCount;

#define SPU_THREAD_PORT        58

namespace UnitTest
{
	TestResults::TestResults(TestReporter* testReporter)
		:mTestReporter(testReporter)
		,mTestSuiteCount(0)
		,mTestFixtureCount(0)
		,mTestCount(0)
		,mFailureCount(0)
	{
	}

	void TestResults::onTestSuiteStart(char const* testName, int inNumTests)
	{
		++mTestSuiteCount;
		if (mTestReporter && mTestSuiteCount > exceptionSuiteIndex )
			mTestReporter->reportTestSuiteStart(inNumTests,testName);
	}
	
	void TestResults::onTestSuiteEnd(char const* testName, float secondsElapsed)
	{
		if (mTestReporter && mTestSuiteCount > exceptionSuiteIndex)
			mTestReporter->reportTestSuiteEnd(testName, secondsElapsed);
	}

	void TestResults::onTestFixtureStart(char const* testName, int inNumTests)
	{
		++mTestFixtureCount;
		if (mTestReporter && mTestFixtureCount > exceptionFixtureIndex)
			mTestReporter->reportTestFixtureStart(inNumTests,testName);
	}

	void TestResults::onTestFixtureEnd(char const* testName, float secondsElapsed)
	{
		if (mTestReporter && mTestFixtureCount > exceptionFixtureIndex)
			mTestReporter->reportTestFixtureEnd(testName, secondsElapsed);
	}

	void TestResults::onTestStart(char const* testName)
	{
		++mTestCount;
		if (mTestReporter && mTestCount > exceptionTestIndex)
		{
			unsigned int data1 = ( ((unsigned int)mTestSuiteCount) << 12 ) | (((unsigned int)mTestFixtureCount) & 0x00000FFF );
			unsigned int data2 = ( ((unsigned int)mTestCount) << 16 ) | (((unsigned int)mFailureCount) & 0x0000FFFF );
			sys_spu_thread_send_event(SPU_THREAD_PORT, data1, data2);
			mTestReporter->reportTestStart(testName);
		}
	}

	void TestResults::onTestFailure(char const* file, int line, char const* testName, char const* failure)
	{
		if (mTestCount < exceptionTestIndex)
		{
			mFailureCount = failureCount;
			return;
		}
		++mFailureCount;
		if (mTestReporter)
			mTestReporter->reportFailure(file, line, testName, failure);
	}

	void TestResults::onTestEnd(char const* testName, float secondsElapsed)
	{
		if (mTestReporter && mTestCount > exceptionTestIndex)
			mTestReporter->reportTestEnd(testName, secondsElapsed);
	}


	int TestResults::getTestCount() const
	{
		return mTestCount;
	}

	int TestResults::getFailureCount() const
	{
		return mFailureCount;
	}
}


#endif //TARGET_PS3_SPU
