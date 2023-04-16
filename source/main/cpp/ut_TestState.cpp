#include "cunittest/private/ut_TestState.h"

namespace UnitTest
{
	TestState::TestState(int testCount, int failureCount)
		:mExceptionTestCount(testCount)
		,mExceptionFailureCount(failureCount)
	{
		mExceptionOccurred = (mExceptionTestCount != 0);
	}

}