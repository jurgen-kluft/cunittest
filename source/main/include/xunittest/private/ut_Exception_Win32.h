#ifndef __X_CORE_UNIT_TEST_ASSERTEXCEPTION_WIN32_H__
#define __X_CORE_UNIT_TEST_ASSERTEXCEPTION_WIN32_H__

#include "xunittest\private\ut_Exception.h"
#include <exception>

namespace UnitTest
{
	class Exception : public std::exception
	{
	public:
		virtual char const*		filename() const = 0;
		virtual int				lineNumber() const = 0;
	};
}

#endif	///< __X_CORE_UNIT_TEST_ASSERTEXCEPTION_WIN32_H__