#include "xunittest\private\ut_TestReporterStdout.h"

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
		StringFormat("%s(%d): error: Failure in %s:%s.\n", file, line, testName, failure);
		Trace(mMessage);
	}

	void TestReporterStdout::reportSummary(float secondsElapsed,int failureCount,int testCount)
	{
		if (failureCount > 0)
			StringFormat("FAILURE: %d out of %d tests failed.\n", failureCount, testCount);
		else
			StringFormat("Success: %d  tests passed.\n", testCount);
		Trace(mMessage);

		StringFormat("Test time: %.2f seconds.\n", secondsElapsed);
		Trace(mMessage);
	}
}
