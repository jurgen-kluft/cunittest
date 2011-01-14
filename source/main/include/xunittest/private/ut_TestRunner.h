#ifndef __XUNITTEST_TESTRUNNER_H__
#define __XUNITTEST_TESTRUNNER_H__

namespace UnitTest
{
	class TestReporter;
	class SuiteNode;

	int		runAllTests(TestReporter& reporter, SuiteNode* inSuiteList, int maxTestTimeInMs = 0);
}

#endif	///< __XUNITTEST_TESTRUNNER_H__
