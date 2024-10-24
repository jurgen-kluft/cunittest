#if defined(TARGET_PC) || defined(TARGET_360)

#include "cunittest/private/ut_Stdout.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI

#ifdef TARGET_PC
#include <windows.h>
#endif

#include <time.h>
#include <stdio.h>

#define STRINGFORMAT			sprintf_s			// Here you can divert to a printf/string-formatting implementation
#define TRACE(a)				printf(a)			// Here you can divert to a log/trace/print implementation

namespace UnitTest
{
	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, double inValue)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, float inValue)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue, int inValue2)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inValue, inValue2);
	}

	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, int inValue, const char* inName)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inValue, inName);
	}

	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inFile, inLine, inTestName, inFailure);
	}

	void Stdout::StringFormat(char* outMessage, int inMessageSizeInBytes, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure, const char* inMessage)
	{
		STRINGFORMAT(outMessage, inMessageSizeInBytes, inFormatStr, inFile, inLine, inTestName, inFailure, inMessage);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, int inValue)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inValue);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inStr3);
	}

	void			Stdout::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3, const char* inStr4)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inStr3, inStr4);
	}

	void Stdout::Trace(const char* inMessage)
	{
		TRACE(inMessage);
		#ifdef TARGET_PC
		#ifdef UNICODE
        ::OutputDebugStringA(inMessage);
#        else
		::OutputDebugString(inMessage);
#        endif
		#endif
	}
}


#endif
