#ifndef __XUNITTEST_TESTSTATE_H__
#define __XUNITTEST_TESTSTATE_H__

namespace UnitTest
{
	class TestState
	{
	public:
		TestState(int testCount = 0, int failureCount = 0);
		~TestState();

		void setTestState(int testCount, int failureCount);

		bool hasExceptionOccurred() { return mExceptionOccurred; }
		bool needToSkipTest(int testCount) { return mExceptionOccurred && (testCount < mExceptionTestCount); }
		bool isTheCrashedTest(int testCount) { return mExceptionOccurred && (testCount == mExceptionTestCount); }
		int  getExceptionFailureCount() { return mExceptionFailureCount; }

	private:
		bool		mExceptionOccurred;
		int			mExceptionTestCount;
		int			mExceptionFailureCount;
	};
}
#endif	///<__XUNITTEST_TESTSTATE_H__
