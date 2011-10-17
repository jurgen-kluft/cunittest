#ifdef TARGET_PC
#include <windows.h>

#include "xunittest\xunittest.h"
#include "xunittest\private\ut_TestReporterStdout.h"
#include "xunittest\private\ut_TestReporterTeamCity.h"

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
	UnitTest::SetCountingAllocator(NULL);
	UnitTestObserver observer;
	UnitTest::SetObserver(&observer);

	UnitTest::TestReporterStdout stdout_reporter;
	UnitTest::TestReporter& reporter = stdout_reporter;

	bool result = gRunUnitTest(reporter);

	return result ? 0 : -1;
}

struct x_WinCmdLine
{
	int			mArgC;
	char**		mArgV;
	char		mFilename[_MAX_PATH];

	void		Parse(LPSTR lpCmdLine);
	void		Destroy();
};

void x_WinCmdLine::Parse(LPSTR lpCmdLine)
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
	char** argv = (char**)malloc(argc * sizeof(char*));

	arg = lpCmdLine;
	int index = 1;

	while (arg[0] != 0)
	{
		while (arg[0] != 0 && arg[0] == ' ')
			arg++;

		if (arg[0] != 0)
		{
			argv[index] = arg;
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
	mArgV = argv;

	// put the program name into argv[0]
	GetModuleFileName(NULL, mFilename, _MAX_PATH);
	argv[0] = mFilename;
}

void x_WinCmdLine::Destroy()
{
	free(mArgV);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	x_WinCmdLine arg;
	arg.Parse(lpCmdLine);
	int r = main(arg.mArgC, arg.mArgV);
	arg.Destroy();
	return r;
}

#endif