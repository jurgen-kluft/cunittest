#ifdef TARGET_PS3_SPU

#include "xunittest\private\ut_Stdout.h"

#include <stdio.h>
#include <cstdio>
#include <spu_printf.h>

#define STRINGFORMAT			snprintf				// Here you can divert to a printf/string-formatting implementation
#define TRACE(a)				spu_printf(a)		// Here you can divert to a log/trace/print implementation

namespace UnitTest
{
	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, float inValue)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, int inValue)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, int inValue, int inValue2)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inValue, inValue2);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, int inValue, const char* inName)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inValue, inName);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inFile, inLine, inTestName, inFailure);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, int inValue)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inValue);
	}

	void Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inStr3);
	}

	void Stdout::Trace(const char* inMessage)
	{
		TRACE(inMessage);
	}
}

#endif