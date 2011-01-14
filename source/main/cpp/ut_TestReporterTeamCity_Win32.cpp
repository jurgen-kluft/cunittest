#ifdef TARGET_PC

#include "xunittest\private\ut_TestReporterTeamCity.h"

#include <stdio.h>
#include <cstdio>

#define STRINGFORMAT			sprintf_s			// Here you can divert to a printf/string-formatting implementation
#define TRACE(a)				printf(a)		// Here you can divert to a log/trace/print implementation

namespace UnitTest
{
	void TestReporterTeamCity::StringFormat(const char* inFormatStr, const char* inName)
	{
		STRINGFORMAT(mMessage, MAX_MESSAGE_LENGTH, inFormatStr, inName);
	}

	void TestReporterTeamCity::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, int inValue)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inValue);
	}

	void TestReporterTeamCity::StringFormat(char* outMessage, int inMaxMessageLength, const char* inFormatStr, const char* inStr1, const char* inStr2, const char* inStr3)
	{
		STRINGFORMAT(outMessage, inMaxMessageLength, inFormatStr, inStr1, inStr2, inStr3);
	}

	void TestReporterTeamCity::Trace(const char* inMessage)
	{
		TRACE(inMessage);
	}
}


#endif
