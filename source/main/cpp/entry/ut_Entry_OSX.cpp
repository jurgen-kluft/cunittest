#ifdef TARGET_OSX
#include <stdlib.h>

#include "xunittest/xunittest.h"
#include "xunittest/private/ut_TestReporterStdout.h"
#include "xunittest/private/ut_TestReporterTeamCity.h"

class UnitTestObserver : public UnitTest::Observer
{
public:
	void	BeginFixture(const char* filename, const char* suite_name, const char* fixture_name)
	{
	}
	void	EndFixture()
	{
	}
};

extern bool gRunUnitTest(UnitTest::TestReporter& reporter);
int main(int argc, char** argv)
{
	UnitTest::SetAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	return result ? 0 : -1;
}

#define _MAX_PATH 1024

struct x_WinCmdLine
{
	int					mArgC;
	const char *		mArgV[128];
	char				mFilename[_MAX_PATH];

	void				Parse(char* lpCmdLine);
	void				Destroy();
};

void x_WinCmdLine::Parse(char* lpCmdLine)
{
	// count the arguments
	int argc = 1;
	char* arg = lpCmdLine;

	while (arg[0] != 0)
	{
		while (arg[0] != 0 && arg[0] == ' ')
			arg++;

		if (arg[0] != 0)
		{
			argc++;
			while (arg[0] != 0 && arg[0] != ' ')
				arg++;
		}
	}    

	// parse the arguments

	arg = lpCmdLine;
	int index = 1;

	while (arg[0] != 0)
	{
		while (arg[0] != 0 && arg[0] == ' ')
			arg++;

		if (arg[0] != 0)
		{
			mArgV[index] = arg;
			index++;

			while (arg[0] != 0 && arg[0] != ' ')
				arg++;

			if (arg[0] != 0)
			{
				arg[0] = 0;    
				arg++;
			}
		}
	}

	mArgC = argc;

	// put the program name into argv[0]
	mArgV[0] = mFilename;
}

void x_WinCmdLine::Destroy()
{
	free(mArgV);
}


#endif