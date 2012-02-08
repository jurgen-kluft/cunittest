#ifndef __XUNITTEST_ASSERTEXCEPTION_PS3_SPU_H__
#define __XUNITTEST_ASSERTEXCEPTION_PS3_SPU_H__

namespace UnitTest
{
	class Exception
	{
	public:
		virtual char const*		filename() const = 0;
		virtual int				lineNumber() const = 0;
	};
}

#endif	///< __XUNITTEST_ASSERTEXCEPTION_PS3_H__
