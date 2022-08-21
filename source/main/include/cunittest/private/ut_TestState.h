#ifndef __CUNITTEST_TESTSTATE_H__
#define __CUNITTEST_TESTSTATE_H__

namespace UnitTest
{
	class TestState
	{
	public:
		~TestState();

		void setTestState(int testCount, int failureCount);

		bool hasExceptionOccurred() { return mExceptionOccurred; }
		bool needToSkipTest(int testCount) { return mExceptionOccurred && (testCount < mExceptionTestCount); }
		bool isTheCrashedTest(int testCount) { return mExceptionOccurred && (testCount == mExceptionTestCount); }
		int  getExceptionFailureCount() { return mExceptionFailureCount; }

		static void sCreateInstance();
		static TestState* sGetInstance();
		static void sDestroyInstance();

	private:
		TestState(int testCount = 0, int failureCount = 0);

		static TestState*	sInstance;

		bool				mExceptionOccurred;
		int					mExceptionTestCount;
		int					mExceptionFailureCount;
	};
}
#endif	///<__XUNITTEST_TESTSTATE_H__
