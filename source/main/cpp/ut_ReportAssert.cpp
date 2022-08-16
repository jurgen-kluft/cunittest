#include "cunittest/private/ut_AssertException.h"

namespace UnitTest
{
	void reportAssert(char const* description, char const* filename, int const lineNumber)
	{
		UT_THROW1(AssertException(description, filename, lineNumber));
	}
}

