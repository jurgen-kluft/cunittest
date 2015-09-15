#include "xunittest/private/ut_AssertException.h"
#include "xunittest/private/ut_Utils.h"

namespace UnitTest
{
	AssertException::AssertException(char const* description, char const* filename, const int lineNumber)
		:mLineNumber(lineNumber)
	{
		gStringCopy(mDescription, description, FILENAME_MAX_STR_LENGTH);
		gStringCopy(mFilename, filename, DESCRIPTION_MAX_STR_LENGTH);
	}

	AssertException::~AssertException() UT_THROW0()
	{
	}

	char const* AssertException::what() const UT_THROW0()
	{
		return mDescription;
	}

	char const* AssertException::filename() const
	{
		return mFilename;
	}

	int AssertException::lineNumber() const
	{
		return mLineNumber;
	}

}

