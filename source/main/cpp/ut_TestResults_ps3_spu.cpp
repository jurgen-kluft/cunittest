#ifdef TARGET_PS3_SPU

#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_TestState.h"

#include <spu_printf.h>

#define SPU_THREAD_PORT        58

namespace UnitTest
{
	TestResults::TestResults(TestReporter* testReporter, TestState* testState)
		:mTestReporter(testReporter)
		,mTestState(testState)
		,mTestSuiteCount(0)
		,mTestFixtureCount(0)
		,mTestCount(0)
		,mFailureCount(0)
	{
		if (mTestState)
		{
			if (mTestState->hasExceptionOccurred())
			{
				mFailureCount = mTestState->getExceptionFailureCount();
			}
		}
	}

	void TestResults::onTestSuiteStart(char const* testName, int inNumTests)
	{
		++mTestSuiteCount;
		if ( mTestReporter )
			mTestReporter->reportTestSuiteStart(inNumTests,testName);
	}
	
	void TestResults::onTestSuiteEnd(char const* testName, float secondsElapsed)
	{
		if ( mTestReporter )
			mTestReporter->reportTestSuiteEnd(testName, secondsElapsed);
	}

	void TestResults::onTestFixtureStart(char const* testName, int inNumTests)
	{
		++mTestFixtureCount;
		if ( mTestReporter )
			mTestReporter->reportTestFixtureStart(inNumTests,testName);
	}

	void TestResults::onTestFixtureEnd(char const* testName, float secondsElapsed)
	{
		if ( mTestReporter )
			mTestReporter->reportTestFixtureEnd(testName, secondsElapsed);
	}

	void TestResults::onTestStart(char const* testName)
	{
		++mTestCount;
		if ( mTestReporter && mTestState && mTestState->hasExceptionOccurred() && mTestState->needToSkipTest(mTestCount) )
		{
			if ( mTestCount > 0xFFFF || mTestCount < 0 || mFailureCount > 0xFFFF || mFailureCount < 0 )
			{
				spu_printf("WARNING: Data overflow warning!\n");
			}
			unsigned int data1 = mTestCount;
			unsigned int data2 = mFailureCount;
			
			int ret = sys_spu_thread_send_event(SPU_THREAD_PORT, data1, data2);

			while (ret != CELL_OK)
			{
				if (ret == ENOTCONN)
				{
					spu_printf("Error sending message to the PPU thread : spup is not connected to any event queue.\n");
					break;
				}
				else if (ret == EINVAL)
				{
					spu_printf("Error sending message to the PPU thread : spup is out of range.\n");
					break;
				}
				else if (ret == EBUSY)
				{
					spu_printf("WARNING IN SENDING MESSAGE TO PPU: The event queue is full. Resending...\n");
					ret = sys_spu_thread_send_event(SPU_THREAD_PORT, data1, data2);
				}
				else
				{
					spu_printf("Unknown error while sending message to the PPU thread!\n");
					break;
				}
			}
			//spu_printf("The counts are : %d\t%d\t%d\t%d\t %s \n", mTestSuiteCount, mTestFixtureCount, mTestCount, mFailureCount, testName);

			mTestReporter->reportTestStart(testName);
		}
	}

	void TestResults::onTestFailure(char const* file, int line, char const* testName, char const* failure)
	{
		if (mTestState && mTestState->hasExceptionOccurred() && mTestState->needToSkipTest(mTestCount))
		{
			return;
		}
		++mFailureCount;
		if (mTestReporter)
			mTestReporter->reportFailure(file, line, testName, failure);
	}

	void TestResults::onTestEnd(char const* testName, float secondsElapsed)
	{
		if (mTestReporter)
			mTestReporter->reportTestEnd(testName, secondsElapsed);
	}

	TestState* TestResults::getTestState() const
	{
		return mTestState;
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
