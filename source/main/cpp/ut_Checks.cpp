#include "xunittest\private\ut_Checks.h"
#include "xunittest\private\ut_Utils.h"

namespace UnitTest
{
	namespace
	{
		void gCheckStringsEqual(TestResults& results, char const* const expected, char const* const actual, char const* const testName, char const* const filename, int const line)
		{
			if (gAreStringsEqual(expected, actual) == false)
			{
				UnitTest::StringBuilder stringBuilder;
				stringBuilder << "Expected " << expected << " but was " << actual;
				results.onTestFailure(filename, line, testName, stringBuilder.getText());
			}
		}
	}

	void checkEqual(TestResults& results, char const* const expected, char const* const actual, char const* const testName, char const* const filename, int const line)
	{
		gCheckStringsEqual(results, expected, actual, testName, filename, line);
	}

	void checkEqual(TestResults& results, char* const expected, char* const actual, char const* const testName, char const* const filename, int const line)
	{
		gCheckStringsEqual(results, expected, actual, testName, filename, line);
	}

	void checkEqual(TestResults& results, char* const expected, char const* const actual, char const* const testName, char const* const filename, int const line)
	{
		gCheckStringsEqual(results, expected, actual, testName, filename, line);
	}

	void checkEqual(TestResults& results, char const* const expected, char* const actual, char const* const testName, char const* const filename, int const line)
	{
		gCheckStringsEqual(results, expected, actual, testName, filename, line);
	}
}
