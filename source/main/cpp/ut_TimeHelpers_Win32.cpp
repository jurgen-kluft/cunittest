#ifdef TARGET_PC

#include "cunittest/private/ut_TimeHelpers.h"

#include <windows.h>
#include <cassert>

namespace UnitTest
{
	static double s_multiplier;
	static LARGE_INTEGER s_base;

	void g_InitTimer()
	{
        LARGE_INTEGER frequency;
		BOOL success = ::QueryPerformanceFrequency(&frequency);
		assert(success);
		success = ::QueryPerformanceCounter(&s_base);
		assert(success);
        s_multiplier = 1000.0 / (double)frequency.QuadPart;
		(void) success;
	}

	time_t g_TimeStart()
	{
		LARGE_INTEGER current;
		BOOL const success = ::QueryPerformanceCounter(&current);
		assert(success);
		(void) success;
        current.QuadPart -= s_base.QuadPart;
        return *(time_t*)(&current.QuadPart);
	}

	double g_GetElapsedTimeInMs(time_t stamp)
	{
		LARGE_INTEGER curTime;
		BOOL const success = ::QueryPerformanceCounter(&curTime);
		assert(success);
		(void) success;

        return double(curTime.QuadPart - s_base.QuadPart + (LONGLONG)(stamp)) * s_multiplier;
	}

	void g_SleepMs(int const ms)
	{
		::Sleep(ms);
	}

}

#endif
