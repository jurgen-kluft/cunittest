#include "xunittest\private\ut_AssertException.h"

namespace UnitTest
{
	void reportAssert(char const* description, char const* filename, int const lineNumber)
	{
		throw AssertException(description, filename, lineNumber);
	}
}

