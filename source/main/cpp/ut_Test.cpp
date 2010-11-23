#include "xunittest\private\ut_Config.h"
#include "xunittest\private\ut_Test.h"
#include "xunittest\private\ut_TestList.h"
#include "xunittest\private\ut_TestResults.h"
#include "xunittest\private\ut_TimeHelpers.h"
#include "xunittest\private\ut_AssertException.h"
#include "xunittest\private\ut_StringBuilder.h"


namespace UnitTest
{
	Test::Test(char const* testName, char const* filename, const int lineNumber)
		:mNext(0)
		,mTestName(testName)
		,mFilename(filename)
		,mLineNumber(lineNumber)
		,mTimeConstraintExempt(false)
	{
	}

	Test::~Test()
	{
	}

	void		Test::setup(TestResults& )
	{
	}

	void		Test::teardown(TestResults& )
	{
	}

	void		Test::runImpl(TestResults& ) const
	{
	}

}
