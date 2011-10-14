#ifndef __XUNITTEST_STDOUT_H__
#define __XUNITTEST_STDOUT_H__

namespace UnitTest
{
	class Stdout
	{
	public:
		static void			StringFormat(char* inMessage, const char* inFormatStr, float inValue);
		static void			StringFormat(char* inMessage, const char* inFormatStr, int inValue);
		static void			StringFormat(char* inMessage, const char* inFormatStr, int inValue, int inValue2);
		static void			StringFormat(char* inMessage, const char* inFormatStr, int inValue, const char* inName);
		static void			StringFormat(char* inMessage, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure);

		static void			Trace(const char* inMessage);
	};

}


#endif	///< __XUNITTEST_STDOUT_H__
