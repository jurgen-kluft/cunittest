#include "xunittest\private\ut_TimeConstraint.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_StringBuilder.h"

namespace UnitTest
{
	TimeConstraint::TimeConstraint(int ms, TestResults& result, char const* filename, int lineNumber, char const* testName)
		:mResult(result)
		,mMaxMs(ms)
		,mFilename(filename)
		,mLineNumber(lineNumber)
		,mTestName(testName)
	{
		mTimer.start();
	}

	TimeConstraint::~TimeConstraint()
	{
		const int totalTimeInMs = mTimer.getTimeInMs();
		if (totalTimeInMs > mMaxMs)
		{
			StringBuilder strBuilder;
			strBuilder << "Time constraint failed. Expected to run test under " << mMaxMs << "ms but took " << totalTimeInMs << "ms.";
			mResult.onTestFailure(mFilename, mLineNumber, mTestName, strBuilder.getText());
		}
	}
}
