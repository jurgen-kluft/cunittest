#ifndef __XUNITTEST_ASSERTEXCEPTION_WIN32_H__
#define __XUNITTEST_ASSERTEXCEPTION_WIN32_H__

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

#endif	///< __XUNITTEST_ASSERTEXCEPTION_WIN32_H__
