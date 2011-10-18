#ifdef TARGET_PC

#include "xunittest\private\ut_Stdout.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI
#include <windows.h>
#include <time.h>
#include <stdio.h>

#define STRINGFORMAT			sprintf			// Here you can divert to a printf/string-formatting implementation
#define TRACE(a)				printf(a)			// Here you can divert to a log/trace/print implementation

namespace UnitTest
{
	void Stdout::StringFormat(char* inMessage, const char* inFormatStr, float inValue)
	{
		STRINGFORMAT(inMessage, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* inMessage, const char* inFormatStr, int inValue)
	{
		STRINGFORMAT(inMessage, inFormatStr, inValue);
	}

	void Stdout::StringFormat(char* inMessage, const char* inFormatStr, int inValue, int inValue2)
	{
		STRINGFORMAT(inMessage, inFormatStr, inValue, inValue2);
	}

	void Stdout::StringFormat(char* inMessage, const char* inFormatStr, int inValue, const char* inName)
	{
		STRINGFORMAT(inMessage, inFormatStr, inValue, inName);
	}

	void Stdout::StringFormat(char* inMessage, const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure)
	{
		STRINGFORMAT(inMessage, inFormatStr, inFile, inLine, inTestName, inFailure);
	}

	void Stdout::Trace(const char* inMessage)
	{
		TRACE(inMessage);
		::OutputDebugString(inMessage);
	}
}


#endif
