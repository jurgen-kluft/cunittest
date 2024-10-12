#ifndef __CUNITTEST_TESTREPORTER_H__
#define __CUNITTEST_TESTREPORTER_H__

namespace UnitTest
{
	class TestReporter
	{
	public:
		virtual			~TestReporter();

		virtual void	reportTestSuiteStart(int numTests, char const* name) = 0;
		virtual void	reportTestSuiteEnd(char const* name, float secondsElapsed) = 0;
		virtual void	reportTestFixtureStart(int numTests, char const* name) = 0;
		virtual void	reportTestFixtureEnd(char const* name, float secondsElapsed) = 0;
		virtual void	reportTestStart(char const* name) = 0;
		virtual void	reportTestEnd(char const* name, float secondsElapsed) = 0;
		virtual void	reportFailure(char const* file, int const line, char const* name, char const* failure)= 0;
		virtual void	reportFailure(char const* file, int const line, char const* name, char const* failure, const char* message)= 0;
		virtual void	reportSummary(float secondsElapsed, int failureCount, int testCount) = 0;
	};
}

#endif	///<__XUNITTEST_TESTREPORTER_H__
