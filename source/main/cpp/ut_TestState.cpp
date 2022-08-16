#include "cunittest/private/ut_TestState.h"

UnitTest::TestState* UnitTest::TestState::sInstance = 0;

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

	void TestState::sCreateInstance()
	{
		if (0 == sInstance)
		{
			sInstance = new TestState();
		}
	}

	TestState* TestState::sGetInstance()
	{
		return sInstance;
	}

	void TestState::sDestroyInstance()
	{
		if (0 != sInstance)
		{
			delete sInstance;
			sInstance = 0;
		}
	}
}