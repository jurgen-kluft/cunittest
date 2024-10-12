#ifdef TARGET_LINUX

#include "cunittest/private/ut_TimeHelpers.h"

#include <chrono>
#include <unistd.h>

namespace UnitTest
{
	static uint64_t s_numer;
	static uint64_t s_denom;
	static uint64_t s_base;

	void g_InitTimer()
	{
        s_base = chrono::steady_clock::now();
	}

	time_t g_TimeStart()
    {
        uint64_t start = (uint64_t)chrono::steady_clock::now() - s_base;
        return *(time_t*)(&start);
	}

	double g_GetElapsedTimeInMs(time_t stamp)
	{
		uint64_t const last = s_base + *(uint64_t*)(&stamp);
		uint64_t const current = chrono::steady_clock::now();
		chrono::duration_cast<chrono::milliseconds>(current - last).count()
		return ms;
	}

	void g_SleepMs(int const ms)
	{
		::sleep(ms * 1000);
	}

}

#endif
