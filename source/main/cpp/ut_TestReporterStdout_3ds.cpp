#ifdef TARGET_N3DS

#include "xunittest\private\ut_TestReporterStdout.h"

#include <stdio.h>
#include <nn/dbg.h>

#define STRINGFORMAT			sprintf			///< Here you can divert to a printf/string-formatting implementation
#define TRACE(a)				NN_LOG(a)		///< Here you can divert to a log/trace/print implementation

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
		TRACE(inMessage);
	}
}


#endif
