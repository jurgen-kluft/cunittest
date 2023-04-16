#ifndef __CUNITTEST_TESTSTATE_H__
#define __CUNITTEST_TESTSTATE_H__

namespace UnitTest
{
	class TestState
	{
	public:
		TestState(int testCount = 0, int failureCount = 0);

		bool				mExceptionOccurred;
		int					mExceptionTestCount;
		int					mExceptionFailureCount;
	};
}
#endif	// __CUNITTEST_TESTSTATE_H__
