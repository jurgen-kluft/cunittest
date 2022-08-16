#ifndef __XUNITTEST_TESTREPORTERSTDOUT_H__
#define __XUNITTEST_TESTREPORTERSTDOUT_H__

#include "cunittest/private/ut_TestReporter.h"

namespace UnitTest
{
	class TestReporterStdout : public TestReporter
	{
		enum EConfig
		{
			MAX_MESSAGE_LENGTH = 1023
		};

	public:
						TestReporterStdout();

	private:
		virtual void	reportTestSuiteStart(int numTests, char const* name);
		virtual void	reportTestSuiteEnd(char const* name, float secondsElapsed);
		virtual void	reportTestFixtureStart(int numTests, char const* name);
		virtual void	reportTestFixtureEnd(char const* name, float secondsElapsed);
		virtual void	reportTestStart(char const* name);
		virtual void	reportTestEnd(char const* name, float secondsElapsed);
		virtual void	reportFailure(char const* file, int const line, char const* name, char const* failure);
		virtual void	reportSummary(float secondsElapsed, int failureCount, int testCount);

		void			StringFormat(const char* inFormatStr, float inValue);
		void			StringFormat(const char* inFormatStr, int inValue);
		void			StringFormat(const char* inFormatStr, int inValue, int inValue2);
		void			StringFormat(const char* inFormatStr, int inValue, const char* inName);
		void			StringFormat(const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure);

		void			Trace(const char* inMessage);

		char			mMessage[MAX_MESSAGE_LENGTH+1];
	};

}


#endif	///< __CORE_UNIT_TEST_TESTREPORTERSTDOUT_H__
