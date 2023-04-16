#ifdef TARGET_PC

#include "cunittest/private/ut_TimeHelpers.h"

#include <windows.h>
#include <cassert>

namespace UnitTest
{
	static LARGE_INTEGER s_frequency;
	static LARGE_INTEGER s_base;

	void g_InitTimer()
	{
		BOOL success = ::QueryPerformanceFrequency(&s_frequency);
		assert(success);
		success = ::QueryPerformanceCounter(&s_base);
		assert(success);
		s_frequency.QuadPart = s_frequency.QuadPart;
		(void) success;
	}

	unsigned int g_TimeStart()
	{
		LARGE_INTEGER current;
		BOOL const success = ::QueryPerformanceCounter(&current);
		assert(success);
		(void) success;
		return current - s_base;
	}

	double g_GetElapsedTimeInMs(unsigned int stamp)
	{
		LARGE_INTEGER baseTime = s_base.QuadPart + stamp;

		LARGE_INTEGER curTime;
		BOOL const success = ::QueryPerformanceCounter(&curTime);
		assert(success);
		(void) success;

		LARGE_INTEGER elapsedTime;
		elapsedTime.QuadPart = curTime.QuadPart - baseTime.QuadPart;

		double const ms = double(elapsedTime.QuadPart) * 1000.0 / (double)frequency->QuadPart;
		return ms;
	}

	void g_SleepMs(int const ms)
	{
		::Sleep(ms);
	}

}

#endif
