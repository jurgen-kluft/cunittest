#ifndef __X_CORE_UNIT_TEST_TESTREPORTER_TEAMCITY_H__
#define __X_CORE_UNIT_TEST_TESTREPORTER_TEAMCITY_H__

#include "xunittest\private\ut_TestReporter.h"

namespace UnitTest
{
	class TestReporterTeamCity : public TestReporter
	{
		enum EConfig
		{
			MAX_MESSAGE_LENGTH = 1023
		};

	public:
						TestReporterTeamCity();

	private:
		virtual void	reportTestSuiteStart(int numTests, char const* name);
		virtual void	reportTestSuiteEnd(char const* name, float secondsElapsed);
		virtual void	reportTestFixtureStart(int numTests, char const* name);
		virtual void	reportTestFixtureEnd(char const* name, float secondsElapsed);
		virtual void	reportTestStart(char const* name);
		virtual void	reportTestEnd(char const* name, float secondsElapsed);
		virtual void	reportFailure(char const* file, int const line, char const* name, char const* failure);
		virtual void	reportSummary(float secondsElapsed, int failureCount, int testCount);

		void			StringFormat(const char* inFormatStr, const char* inName);
		void			StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, int inValue);
		void			StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3);

		void			Trace(const char* inMessage);

		char			mMessage[MAX_MESSAGE_LENGTH+1];
	};

}


#endif	///< __X_CORE_UNIT_TEST_TESTREPORTER_TEAMCITY_H__