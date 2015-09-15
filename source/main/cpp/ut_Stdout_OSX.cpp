#if defined(TARGET_OSX)

#include "xunittest/private/ut_Stdout.h"

#include <time.h>
#include <stdio.h>

#define MESSAGE_S(str, size)	str
#define STRINGFORMAT			sprintf				// Here you can divert to a printf/string-formatting implementation

namespace UnitTest
{
	void Stdout::StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, float inValue)
	{
		STRINGFORMAT(MESSAGE_S(inMessage, inMessageSizeInBytes), inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue)
	{
		STRINGFORMAT(MESSAGE_S(inMessage, inMessageSizeInBytes), inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue, int inValue2)
	{
		STRINGFORMAT(MESSAGE_S(inMessage, inMessageSizeInBytes), inFormatStr, inValue, inValue2);
	}

	void Stdout::StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue, const char* inName)
	{
		STRINGFORMAT(MESSAGE_S(inMessage, inMessageSizeInBytes), inFormatStr, inValue, inName);
	}

	void Stdout::StringFormat(char* inMessage, int inMessageSizeInBytes, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure)
	{
		STRINGFORMAT(MESSAGE_S(inMessage, inMessageSizeInBytes), inFormatStr, inFile, inLine, inTestName, inFailure);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1)
	{
		STRINGFORMAT(MESSAGE_S(outMessage, inMessageSizeInBytes), inFormatStr, inStr1);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, int inValue)
	{
		STRINGFORMAT(MESSAGE_S(outMessage, inMessageSizeInBytes), inFormatStr, inStr1, inStr2, inValue);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3)
	{
		STRINGFORMAT(MESSAGE_S(outMessage, inMessageSizeInBytes), inFormatStr, inStr1, inStr2, inStr3);
	}


	void Stdout::Trace(const char* inMessage)
	{
		printf("%s", inMessage);
	}
}


#endif
