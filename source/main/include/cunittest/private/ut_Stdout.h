#ifndef __CUNITTEST_STDOUT_H__
#define __CUNITTEST_STDOUT_H__

namespace UnitTest
{
	class Stdout
	{
	public:
		static void			StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, float inValue);
		static void			StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue);
		static void			StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue, int inValue2);
		static void			StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue, const char* inName);
		static void			StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure);

		static void			StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1);
		static void			StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, int inValue);
		static void			StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3);

		static void			Trace(const char* inMessage);
	};

}


#endif	///< __CUNITTEST_STDOUT_H__
