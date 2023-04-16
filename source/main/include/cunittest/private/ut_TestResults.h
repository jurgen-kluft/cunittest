#ifndef __CUNITTEST_TESTRESULTS_H__
#define __CUNITTEST_TESTRESULTS_H__

namespace UnitTest
{
	class TestReporter;
	class TestState;

	class TestResults
	{
	public:
		explicit		TestResults(TestReporter* reporter = 0, TestState* state = 0);

		void			onTestSuiteStart(char const* testName, int inNumTests);
		void			onTestSuiteEnd(char const* testName, float secondsElapsed);
		void			onTestFixtureStart(char const* testName, int inNumTests);
		void			onTestFixtureEnd(char const* testName, float secondsElapsed);
		void			onTestStart(char const* testName);
		void			onTestFailure(char const* file, int line, char const* testName, char const* failure);
		void			onTestEnd(char const* testName, float secondsElapsed);

		TestReporter*	mTestReporter;
		TestState*		mTestState;
		int				mTestSuiteCount;
		int				mTestFixtureCount;
		int				mTestCount;
		int				mFailureCount;
	};
}

#endif	// __CUNITTEST_TESTRESULTS_H__
