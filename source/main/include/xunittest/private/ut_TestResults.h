#ifndef __X_CORE_UNIT_TEST_TESTRESULTS_H__
#define __X_CORE_UNIT_TEST_TESTRESULTS_H__

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

	private:
		TestReporter*	mTestReporter;
		int				mTestSuiteCount;
		int				mTestFixtureCount;
		int				mTestCount;
		int				mFailureCount;

		// Copy Constructor & Assignment operator are private
						TestResults(TestResults const&);
		TestResults&	operator =(TestResults const&);
	};
}

#endif	///< __X_CORE_UNIT_TEST_TESTRESULTS_H__