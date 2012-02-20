#include "xunittest\private\ut_TestState.h"

namespace UnitTest
{
	TestState::TestState(int testCount, int failureCount)
		:mExceptionTestCount(testCount)
		,mExceptionFailureCount(failureCount)
	{
		mExceptionOccurred = (mExceptionTestCount != 0);
	}

	TestState::~TestState() 
	{
	}

	void TestState::setTestState(int testCount, int failureCount)
	{
		mExceptionTestCount = testCount;
		mExceptionFailureCount = failureCount;
		mExceptionOccurred = (mExceptionTestCount != 0);	
	}
}