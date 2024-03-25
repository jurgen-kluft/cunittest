#include "cunittest/private/ut_TestReporterStdout.h"
#include "cunittest/private/ut_Stdout.h"

namespace UnitTest
{
	TestReporterStdout::TestReporterStdout()
	{
	}

	void TestReporterStdout::reportTestSuiteStart(int numTests, char const* name)
	{
		StringFormat("TestSuite(%d): %s.\n", numTests, name);
		Trace(mMessage);
	}

	void TestReporterStdout::reportTestSuiteEnd(char const* name, float secondsElapsed)
	{
	}

	void TestReporterStdout::reportTestFixtureStart(int numTests, char const* name)
	{
		StringFormat("\tTestFixture(%d): %s.\n", numTests, name);
		Trace(mMessage);
	}

	void TestReporterStdout::reportTestFixtureEnd(char const* name, float secondsElapsed)
	{
	}

	void TestReporterStdout::reportTestStart(char const* name)
	{
	}

	void TestReporterStdout::reportTestEnd(char const* name, float secondsElapsed)
	{
	}

	void TestReporterStdout::reportFailure(char const* file, int const line, char const* testName, char const* failure)
	{
#ifdef TARGET_MAC
		StringFormat("%s:%d: error: Failure in %s:%s.\n", file, line, testName, failure);
#else
		StringFormat("%s(%d): error: Failure in %s:%s.\n", file, line, testName, failure);
#endif
		Trace(mMessage);
	}

	void TestReporterStdout::reportSummary(float secondsElapsed,int failureCount,int testCount)
	{
		if (failureCount > 0)
			StringFormat("FAILURE: %d out of %d tests failed.\n", failureCount, testCount);
		else
			StringFormat("Success: %d tests passed.\n", testCount);
		Trace(mMessage);

		StringFormat("Test time: %.2f seconds.\n", secondsElapsed);
		Trace(mMessage);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, float inValue)
	{
		Stdout::StringFormat(mMessage, MAX_MESSAGE_LENGTH, inFormatStr, inValue);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, int inValue)
	{
		Stdout::StringFormat(mMessage, MAX_MESSAGE_LENGTH, inFormatStr, inValue);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, int inValue, int inValue2)
	{
		Stdout::StringFormat(mMessage, MAX_MESSAGE_LENGTH, inFormatStr, inValue, inValue2);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, int inValue, const char* inName)
	{
		Stdout::StringFormat(mMessage, MAX_MESSAGE_LENGTH, inFormatStr, inValue, inName);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure)
	{
		Stdout::StringFormat(mMessage, MAX_MESSAGE_LENGTH, inFormatStr, inFile, inLine, inTestName, inFailure);
	}

	void TestReporterStdout::Trace(const char* inMessage)
	{
		Stdout::Trace(inMessage);
	}
}
