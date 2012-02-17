#ifndef __XUNITTEST_TESTRESULTS_H__
#define __XUNITTEST_TESTRESULTS_H__

namespace UnitTest
{
	class TestReporter;

	class TestResults
	{
	public:
		explicit		TestResults(TestReporter* reporter = 0);

		void			onTestSuiteStart(char const* testName, int inNumTests);
		void			onTestSuiteEnd(char const* testName, float secondsElapsed);
		void			onTestFixtureStart(char const* testName, int inNumTests);
		void			onTestFixtureEnd(char const* testName, float secondsElapsed);
		void			onTestStart(char const* testName);
		void			onTestFailure(char const* file, int line, char const* testName, char const* failure);
		void			onTestEnd(char const* testName, float secondsElapsed);

		int				getTestCount() const;
		int				getFailureCount() const;

#ifdef TARGET_PS3_SPU
		void			setExceptionState();
		bool			isToBeSkipped();

		// Check if we are now about to run the test that crashed in the previous run.
		bool			isTheCrashedTest();
#endif

	private:
		TestReporter*	mTestReporter;
		int				mTestSuiteCount;
		int				mTestFixtureCount;
		int				mTestCount;
		int				mFailureCount;

#ifdef TARGET_PS3_SPU
		// Variables for SPU only, see if any SPU exception has occurred before this run
		bool			mExceptionDetected;
#endif

		// Copy Constructor & Assignment operator are private
						TestResults(TestResults const&);
		TestResults&	operator =(TestResults const&);
	};
}

#endif	///< __XUNITTEST_TESTRESULTS_H__
