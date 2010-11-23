#ifndef __X_CORE_UNIT_TEST_TESTRUNNER_H__
#define __X_CORE_UNIT_TEST_TESTRUNNER_H__

namespace UnitTest
{
	class TestReporter;
	class SuiteNode;

	int		runAllTests(TestReporter& reporter, SuiteNode* inSuiteList, int maxTestTimeInMs = 0);
}

#endif	///< __X_CORE_UNIT_TEST_TESTRUNNER_H__