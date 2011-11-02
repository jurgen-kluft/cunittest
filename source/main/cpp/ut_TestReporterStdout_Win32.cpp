#ifdef TARGET_PC

#include "xunittest\private\ut_TestReporterStdout.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI
#include <windows.h>
#include <time.h>
#include <stdio.h>

#define STRINGFORMAT			sprintf_s			///< Here you can divert to a printf/string-formatting implementation
#define TRACE(a)				printf(a)			///< Here you can divert to a log/trace/print implementation

namespace UnitTest
{
	void TestReporterStdout::StringFormat(const char* inFormatStr, float inValue)
	{
		STRINGFORMAT(mMessage, inFormatStr, inValue);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, int inValue)
	{
		STRINGFORMAT(mMessage, inFormatStr, inValue);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, int inValue, int inValue2)
	{
		STRINGFORMAT(mMessage, inFormatStr, inValue, inValue2);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, int inValue, const char* inName)
	{
		STRINGFORMAT(mMessage, inFormatStr, inValue, inName);
	}

	void TestReporterStdout::StringFormat(const char* inFormatStr, const char* inFile, int inLine, const char* inTestName, const char* inFailure)
	{
		STRINGFORMAT(mMessage, inFormatStr, inFile, inLine, inTestName, inFailure);
	}


	void TestReporterStdout::Trace(const char* inMessage)
	{
		::OutputDebugString(inMessage);
		TRACE(inMessage);
	}
}


#endif
