#ifndef __X_CORE_UNIT_TEST_ASSERTEXCEPTION_PS3_H__
#define __X_CORE_UNIT_TEST_ASSERTEXCEPTION_PS3_H__

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

#endif	///< __X_CORE_UNIT_TEST_ASSERTEXCEPTION_PS3_H__