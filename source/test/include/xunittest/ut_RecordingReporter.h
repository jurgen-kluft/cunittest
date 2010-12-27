#ifndef __X_CORE_UNITTEST_RECORDINGREPORTER_H__
#define __X_CORE_UNITTEST_RECORDINGREPORTER_H__

#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_Utils.h"

namespace UnitTest
{
	struct RecordingReporter : public TestReporter
	{
	private:
		enum 
		{
			MAX_STRING_LENGTH = 256 
		};

	public:
		RecordingReporter()
			: testRunCount(0)
			, testFailedCount(0)
			, lastFailedLine(0)
			, testFinishedCount(0)
			, lastFinishedTestTime(0)
			, summaryTestCount(0)
			, summaryFailureCount(0)
			, summarySecondsElapsed(0)
		{
		}

		virtual void reportTestSuiteStart(int inNumTests,char const* inName)
		{

		}

		virtual void reportTestSuiteEnd(char const* inName, float secondsElapsed)
		{

		}

		virtual void reportTestFixtureStart(int inNumTests,char const* inName)
		{

		}

		virtual void reportTestFixtureEnd(char const* inName, float secondsElapsed)
		{

		}

		virtual void reportTestStart(char const* testName)
		{
			++testRunCount;
			gStringCopy(lastStartedTest, testName, MAX_STRING_LENGTH-1);
		}

		virtual void reportFailure(char const* file, int const line, char const* testName, char const* failure)
		{
			++testFailedCount;
			gStringCopy(lastFailedFile, file, MAX_STRING_LENGTH-1);
			lastFailedLine = line;
			gStringCopy(lastFailedTest, testName, MAX_STRING_LENGTH-1);
			gStringCopy(lastFailedMessage, failure, MAX_STRING_LENGTH-1);
		}

		virtual void reportTestEnd(char const* testName, float testDuration)
		{
			++testFinishedCount;
			gStringCopy(lastFinishedTest, testName, MAX_STRING_LENGTH-1);
			lastFinishedTestTime = testDuration;
		}

		virtual void reportSummary(float secondsElapsed, int failureCount, int testCount) 
		{
			summaryTestCount = testCount;
			summaryFailureCount = failureCount;
			summarySecondsElapsed = secondsElapsed;
		}

		int testRunCount;
		char lastStartedTest[MAX_STRING_LENGTH];

		int testFailedCount;
		char lastFailedFile[MAX_STRING_LENGTH];
		int lastFailedLine;
		char lastFailedTest[MAX_STRING_LENGTH];
		char lastFailedMessage[MAX_STRING_LENGTH];

		int testFinishedCount;
		char lastFinishedTest[MAX_STRING_LENGTH];
		float lastFinishedTestTime;

		int summaryTestCount;
		int summaryFailureCount;
		float summarySecondsElapsed;
	};

}

#endif	///< __X_CORE_UNITTEST_RECORDINGREPORTER_H__