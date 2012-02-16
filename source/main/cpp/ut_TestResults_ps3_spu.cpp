#ifdef TARGET_PS3_SPU

#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TestReporter.h"

#include <spu_printf.h>

extern unsigned int gExceptionSuiteCount;
extern unsigned int gExceptionFixtureCount;
extern unsigned int gExceptionTestCount;
extern unsigned int gExceptionFailureCount;

#define SPU_THREAD_PORT        58

namespace UnitTest
{
	TestResults::TestResults(TestReporter* testReporter)
		:mTestReporter(testReporter)
		,mTestSuiteCount(0)
		,mTestFixtureCount(0)
		,mTestCount(0)
		,mFailureCount(0)
		,mExceptionDetected(false)
	{
	}

	void TestResults::onTestSuiteStart(char const* testName, int inNumTests)
	{
		++mTestSuiteCount;
		if ( mTestReporter && !(mExceptionDetected && mTestSuiteCount <= gExceptionSuiteCount) )
			mTestReporter->reportTestSuiteStart(inNumTests,testName);
	}
	
	void TestResults::onTestSuiteEnd(char const* testName, float secondsElapsed)
	{
		if ( mTestReporter && !(mExceptionDetected && mTestSuiteCount <= gExceptionSuiteCount) )
			mTestReporter->reportTestSuiteEnd(testName, secondsElapsed);
	}

	void TestResults::onTestFixtureStart(char const* testName, int inNumTests)
	{
		++mTestFixtureCount;
		if ( mTestReporter && !(mExceptionDetected && mTestFixtureCount <= gExceptionFixtureCount) )
			mTestReporter->reportTestFixtureStart(inNumTests,testName);
	}

	void TestResults::onTestFixtureEnd(char const* testName, float secondsElapsed)
	{
		if ( mTestReporter && !(mExceptionDetected && mTestFixtureCount <= gExceptionFixtureCount) )
			mTestReporter->reportTestFixtureEnd(testName, secondsElapsed);
	}

	void TestResults::onTestStart(char const* testName)
	{
		++mTestCount;
		if ( mTestReporter && !(mExceptionDetected && mTestCount <= gExceptionTestCount) )
		{
			unsigned int data1 = ( ((unsigned int)mTestSuiteCount) << 12 ) | (((unsigned int)mTestFixtureCount) & 0x00000FFF );
			unsigned int data2 = ( ((unsigned int)mTestCount) << 16 ) | (((unsigned int)mFailureCount) & 0x0000FFFF );
			int ret;
			ret = sys_spu_thread_send_event(SPU_THREAD_PORT, data1, data2);

			if (ret != CELL_OK)
			{
				spu_printf("Error sending message to the PPU thread!\n");
			}
			//spu_printf("The counts are : %d\t%d\t%d\t%d\t %s \n", mTestSuiteCount, mTestFixtureCount, mTestCount, mFailureCount, testName);

			mTestReporter->reportTestStart(testName);
		}
	}

	void TestResults::onTestFailure(char const* file, int line, char const* testName, char const* failure)
	{
		if (mExceptionDetected && mTestCount < gExceptionTestCount)
		{
			mFailureCount = gExceptionFailureCount;
			return;
		}
		++mFailureCount;
		if (mTestReporter)
			mTestReporter->reportFailure(file, line, testName, failure);
	}

	void TestResults::onTestEnd(char const* testName, float secondsElapsed)
	{
		if ( mTestReporter && !(mExceptionDetected && mTestCount <= gExceptionTestCount) )
			mTestReporter->reportTestEnd(testName, secondsElapsed);
	}


	void TestResults::setExceptionState()
	{
		if (0 != gExceptionTestCount)
		{
			mExceptionDetected = true;
			mFailureCount = gExceptionFailureCount;
		}
	}

	bool TestResults::isToBeSkipped()
	{
		if (mExceptionDetected && mTestCount < gExceptionTestCount)
			return true;
		return false;
	}

	bool TestResults::isTheCrashedTest()
	{
		if (mExceptionDetected && mTestCount == gExceptionTestCount)
			return true;
		return false;
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
