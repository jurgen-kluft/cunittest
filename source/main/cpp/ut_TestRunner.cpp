#include "xunittest\private\ut_Config.h"
#include "xunittest\private\ut_TestRunner.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_Test.h"
#include "xunittest\private\ut_TestMacros.h"
#include "xunittest\private\ut_TestList.h"
#include "xunittest\private\ut_TestReporter.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TimeHelpers.h"
#include "xunittest\private\ut_StringBuilder.h"
#include "xunittest\private\ut_Stdout.h"

namespace UnitTest
{
	int runAllTests(TestReporter& reporter, SuiteNode* inSuiteList, int maxTestTimeInMs)
	{
		TestResults result(&reporter);

		Timer overallTimer;
		overallTimer.start();

		SuiteNode* suiteList = inSuiteList;
		while (suiteList!=0)
		{
			int numTests = 0;
			TestSuite* curTestSuite = suiteList->mTestSuite;
			TestFixture* curTestFixture = curTestSuite->getFixtures();
			while (curTestFixture != 0)
			{
				++numTests;
				curTestFixture = curTestFixture->getNext();
			}

			float suiteSecondsElapsed = overallTimer.getTimeInMs() / 1000.0f;
			result.onTestSuiteStart(curTestSuite->getName(), numTests);

			curTestFixture = curTestSuite->getFixtures();
			while (curTestFixture != 0)
			{
				GetObserver()->BeginFixture(curTestFixture->mFilename, curTestSuite->getName(), curTestFixture->mTestName);
				int iAllocCnt = GetNumAllocations();
				curTestFixture->run(result, maxTestTimeInMs);
				GetObserver()->EndFixture();
				curTestFixture = curTestFixture->getNext();
			}

			suiteSecondsElapsed = (overallTimer.getTimeInMs() / 1000.0f) - suiteSecondsElapsed;
			result.onTestSuiteEnd(curTestSuite->getName(), suiteSecondsElapsed);

			suiteList = suiteList->mNext;
		}

		float const secondsElapsed = overallTimer.getTimeInMs() / 1000.0f;
		reporter.reportSummary(secondsElapsed, result.getFailureCount(), result.getTestCount());

		return result.getFailureCount();
	}
}
