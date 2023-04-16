#ifndef __CUNITTEST_TESTRUNNER_H__
#define __CUNITTEST_TESTRUNNER_H__

namespace UnitTest
{
	class TestContext;
	class TestReporter;

	struct TestSuite;

	int		runAllTests(TestContext& context, TestReporter& reporter, TestSuite* inSuiteList, int maxTestTimeInMs = 0);
}

#endif	///< __CUNITTEST_TESTRUNNER_H__
