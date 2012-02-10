#ifndef __XUNITTEST_ASSERTEXCEPTION_3DS_H__
#define __XUNITTEST_ASSERTEXCEPTION_3DS_H__

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

#endif	///< __XUNITTEST_ASSERTEXCEPTION_3DS_H__
