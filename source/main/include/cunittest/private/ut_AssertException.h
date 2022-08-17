#ifndef __XUNITTEST_ASSERTEXCEPTION_H__
#define __XUNITTEST_ASSERTEXCEPTION_H__

#include "cunittest/private/ut_Exception.h"

namespace UnitTest
{
	class AssertException : public Exception
	{
		enum ESettings
		{
			DESCRIPTION_MAX_STR_LENGTH = 512, 
			FILENAME_MAX_STR_LENGTH = 256 
		};

	public:
								AssertException(char const* description, char const* filename, const int lineNumber);
		virtual					~AssertException() UT_THROW0();

		virtual char const*		what() const UT_THROW0();

		virtual char const*		filename() const;
		virtual int				lineNumber() const;

	private:
		char					mDescription[DESCRIPTION_MAX_STR_LENGTH];
		char					mFilename[FILENAME_MAX_STR_LENGTH];
		int						mLineNumber;
	};
}

#endif	///< __CORE_UNIT_TEST_ASSERTEXCEPTION_H__
